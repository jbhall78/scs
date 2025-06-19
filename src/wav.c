#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h> // Already included in wav.h, but good to ensure
#ifdef _WIN32
#include <windows.h>
#include <io.h> // For _open, _read, _close
#include <sys/stat.h> // For _stat
#define stat _stat
#define open _open
#define read _read
#define close _close
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif

#include "wav.h"

/* enable untested code? */
#define UNTESTED_EXTENDED_WAV_FORMAT // More descriptive

/* enable debug output? */
#undef DEBUG

#ifdef DEBUG
#define dprintf(...) printf(__VA_ARGS__)
#else
#define dprintf(...)
#endif

// --- Endianness Helper (replace with SDL_SwapLE16/32 if using SDL) ---
#if defined(__linux__) || defined(__APPLE__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__)
#include <endian.h> // For le16toh, le32toh
#elif defined(_WIN32)
#include <intrin.h> // For _byteswap_ushort, _byteswap_ulong
#define le16toh(x) ((unsigned short)x) // Assume LE on Windows for now, or use _byteswap_ushort if needed
#define le32toh(x) ((unsigned long)x)  // Assume LE on Windows for now, or use _byteswap_ulong if needed
// If your Windows target is truly big-endian or mixed (unlikely), you'd need actual swapping.
#else
// Fallback for other systems, assume Little-Endian or define custom swap
#define le16toh(x) (x)
#define le32toh(x) (x)
#warning "Unsure of system endianness. Assuming Little-Endian for WAV parsing."
#endif

// --- Macros with bounds checking and endian conversion ---
// Pass current pointer 'p', start of buffer 'data', and total length 'len'
// Returns 0 on success, -1 on error (out of bounds)
#define READ_UINT16_LE(val_ptr) do { \
    if (p - data + sizeof(uint16_t) > len) { \
        fprintf(stderr, "Error: Read past end of file for uint16_t.\n"); \
        return NULL; /* Or handle error more gracefully */ \
    } \
    memcpy(val_ptr, p, sizeof(uint16_t)); \
    *(uint16_t*)val_ptr = le16toh(*(uint16_t*)val_ptr); \
    p += sizeof(uint16_t); \
} while(0)

#define READ_UINT32_LE(val_ptr) do { \
    if (p - data + sizeof(uint32_t) > len) { \
        fprintf(stderr, "Error: Read past end of file for uint32_t.\n"); \
        return NULL; /* Or handle error more gracefully */ \
    } \
    memcpy(val_ptr, p, sizeof(uint32_t)); \
    *(uint32_t*)val_ptr = le32toh(*(uint32_t*)val_ptr); \
    p += sizeof(uint32_t); \
} while(0)

#define SKIP_BYTES(bytes_to_skip) do { \
    if (p - data + bytes_to_skip > len) { \
        fprintf(stderr, "Error: Skip past end of file.\n"); \
        return NULL; /* Or handle error more gracefully */ \
    } \
    p += bytes_to_skip; \
} while(0)

static int
read_file(const char *filename, void **_data, uint64_t *_len) // Use const char*
{
    int fd;
    struct stat st;
    uint64_t len;
    char *data;

    /* get file size */
    if (stat(filename, &st) < 0) {
        fprintf(stderr, "cannot stat wav file: %s: %s\n",
                filename, strerror(errno));
        return -1;
    }
    len = st.st_size;

    /* open file */
    if ((fd = open(filename, O_RDONLY
    #ifdef _WIN32 // Add binary mode for Windows
    | O_BINARY
    #endif
    )) < 0) {
        fprintf(stderr, "error opening wav file: %s: %s\n",
                filename, strerror(errno));
        return -1;
    }

    /* allocate space */
    data = malloc(len);
    if (data == NULL) {
        close(fd); // Don't forget to close on malloc fail
        return -1; // errno should already be set by malloc
    }
    // memset(data, 0, len); // Not strictly necessary if data is immediately overwritten

    /* read file */
    ssize_t bytes_read_total = 0;
    while (bytes_read_total < len) {
        ssize_t current_read = read(fd, data + bytes_read_total, len - bytes_read_total);
        if (current_read == 0) { // EOF
            fprintf(stderr, "warning: unexpected EOF while reading file: %s (read %lu of %lu bytes)\n",
                    filename, bytes_read_total, len);
            break; // Break the loop, caller should check total_read != len
        }
        if (current_read < 0) { // Error
            fprintf(stderr, "cannot read file: %s: %s\n",
                    filename, strerror(errno));
            close(fd);
            free(data);
            return -1;
        }
        bytes_read_total += current_read;
    }

    /* close file */
    close(fd);

    if (bytes_read_total != len) {
        fprintf(stderr, "Error: Incomplete read of file: %s (expected %lu bytes, read %lu bytes)\n",
                filename, len, bytes_read_total);
        free(data);
        return -1;
    }

    *_len = len;
    *_data = data;

    return 0; // Return 0 for success, -1 for error (common convention)
}


wav_t *
wav_new(const char *filename) // Use const char*
{
    wav_t *wav = NULL;
    char *buf = NULL, *data = NULL;
    char *p = NULL; // Current parsing pointer
    uint64_t len = 0;
    uint32_t chunk_id;
    uint32_t chunk_data_size;
    uint16_t n16;
    uint32_t n32;
    uint8_t guid[16]; // For the GUID in extended format

    /* read file into memory */
    if (read_file(filename, (void **)&data, &len) < 0)
        goto end;

    p = data; // Initialize parsing pointer

    /* allocate object */
    wav = malloc(sizeof(wav_t));
    if (wav == NULL) {
        fprintf(stderr, "Error: Failed to allocate wav_t structure: %s\n", strerror(errno));
        goto end;
    }
    memset(wav, 0, sizeof(wav_t));

    // Check minimum file size for RIFF header
    if (len < 12) { // RIFF(4) + Size(4) + WAVE(4)
        fprintf(stderr, "Error: WAV file too small to be valid: %s\n", filename);
        goto end;
    }

    // RIFF chunk
    READ_UINT32_LE(&chunk_id); // "RIFF"
    if (chunk_id != 0x46464952) { // 'RIFF' in Little-Endian
        fprintf(stderr, "Error: Not a RIFF file (expected 0x46464952, got 0x%X): %s\n", chunk_id, filename);
        goto end;
    }
    dprintf("id: RIFF\n");

    READ_UINT32_LE(&chunk_data_size); // File size - 8 bytes
    dprintf("RIFF size: %u\n", chunk_data_size);
    // You could validate if chunk_data_size + 8 == len

    READ_UINT32_LE(&chunk_id); // "WAVE"
    if (chunk_id != 0x45564157) { // 'WAVE' in Little-Endian
        fprintf(stderr, "Error: Not a WAV file (expected 0x45564157, got 0x%X): %s\n", chunk_id, filename);
        goto end;
    }
    dprintf("id: WAVE\n");


    while (p - data < len) {
        // Read chunk ID
        if (p - data + sizeof(uint32_t) > len) {
            dprintf("Warning: Reached end of file unexpectedly while looking for next chunk ID.\n");
            break; // No more full chunk IDs to read
        }
        READ_UINT32_LE(&chunk_id);

        // Read chunk size
        if (p - data + sizeof(uint32_t) > len) {
            dprintf("Warning: Reached end of file unexpectedly while looking for next chunk size.\n");
            break; // No more full chunk sizes to read
        }
        READ_UINT32_LE(&chunk_data_size);

        dprintf("Chunk ID: 0x%X, Size: %u\n", chunk_id, chunk_data_size);

        // Check if skipping this chunk would go past end of buffer
        if (p - data + chunk_data_size > len) {
            fprintf(stderr, "Error: Malformed WAV file, chunk data extends past end of file for chunk 0x%X. (Remaining %lu, Chunk size %u)\n",
                    chunk_id, len - (p - data), chunk_data_size);
            goto end;
        }

        switch(chunk_id) {
            case 0x20746D66: /* 'fmt ' */
                dprintf("id: fmt \n");

                // Basic validation of fmt chunk size
                if (chunk_data_size < 16) { // Minimum fmt chunk size for PCM
                    fprintf(stderr, "Error: 'fmt ' chunk too small (%u bytes). Minimum is 16. %s\n", chunk_data_size, filename);
                    goto end;
                }

                READ_UINT16_LE(&n16); // Format code
                dprintf("format: %d\n", n16);
                // Store format if you want to support non-PCM (e.g. ADPCM, float)
                // if (n16 != 1 && n16 != 0xFFFE) { // 1 = PCM, 0xFFFE = WAVE_FORMAT_EXTENSIBLE
                //     fprintf(stderr, "Warning: Unsupported audio format %d. Only PCM (1) and EXTENSIBLE (0xFFFE) supported.\n", n16);
                //     // Might choose to exit or just skip this chunk's data
                // }


                READ_UINT16_LE(&n16); // Channels
                dprintf("channels: %d\n", n16);
                wav->channels = n16;

                READ_UINT32_LE(&n32); // Sample rate
                dprintf("sample rate: %d\n", n32);
                wav->sample_rate = n32;

                READ_UINT32_LE(&n32); // Byte rate
                dprintf("byte rate: %d\n", n32);
                wav->byte_rate = n32;

                READ_UINT16_LE(&n16); // Block align
                dprintf("block align: %d\n", n16);
                wav->block_align = n16;

                READ_UINT16_LE(&n16); // Bits per sample
                dprintf("bits per sample: %d\n", n16);
                wav->bits_per_sample = n16;

#ifdef UNTESTED_EXTENDED_WAV_FORMAT
                if (chunk_data_size > 16) {
                    READ_UINT16_LE(&n16); // cbSize (extension size)
                    dprintf("cbSize: %d\n", n16);

                    // For WAVE_FORMAT_EXTENSIBLE (0xFFFE), cbSize is usually 22.
                    // The actual format code is then within the GUID sub-format.
                    if (chunk_data_size >= 40 && n16 >= 22) { // 16 bytes (base) + 2 (cbSize) + 22 (extension) = 40
                        READ_UINT16_LE(&n16); // wValidBitsPerSample / wSamplesPerBlock
                        dprintf("valid bits per sample / samples per block: %d\n", n16);

                        READ_UINT32_LE(&n32); // Channel mask
                        dprintf("channel mask: 0x%X\n", n32);
                        wav->channel_mask = n32;

                        // SubFormat GUID (16 bytes)
                        if (p - data + sizeof(guid) > len) {
                            fprintf(stderr, "Error: Read past end of file for GUID.\n");
                            goto end;
                        }
                        memcpy(guid, p, sizeof(guid)); // GUID is usually byte-for-byte, not endian-swapped
                        p += sizeof(guid);
                        // You'd typically check guid[0] for 0x01 (PCM) or other formats here
                        dprintf("SubFormat GUID: %02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x\n",
                                guid[0], guid[1], guid[2], guid[3], guid[4], guid[5], guid[6], guid[7],
                                guid[8], guid[9], guid[10], guid[11], guid[12], guid[13], guid[14], guid[15]);
                    }
                    // Skip any remaining bytes in fmt chunk if cbSize or total chunk_data_size was larger
                    if (chunk_data_size > (p - (data + (p - data - chunk_data_size)))) { // Check if we read less than chunk_data_size
                        SKIP_BYTES(chunk_data_size - (p - (data + (p - data - chunk_data_size))));
                    }
                }
#else // If UNTESTED_EXTENDED_WAV_FORMAT is not defined, ensure we skip any extra bytes
                if (chunk_data_size > 16) {
                    SKIP_BYTES(chunk_data_size - 16);
                }
#endif
                break;

            case 0x74636166: /* 'fact' */
                dprintf("id: fact\n");
                // The 'fact' chunk is optional and contains information about
                // the number of samples if the format involves compression.
                // For PCM, it's typically either absent or has 4 bytes (dwSampleLength).
                if (chunk_data_size >= 4) {
                    READ_UINT32_LE(&n32); // dwSampleLength
                    dprintf("fact_samples: %u\n", n32);
                    // Store this if you need exact sample count for compressed formats
                    SKIP_BYTES(chunk_data_size - 4); // Skip any remaining
                } else {
                    SKIP_BYTES(chunk_data_size);
                }
                break;

            case 0x61746164: /* 'data' */
                dprintf("id: data\n");
                wav->data_size = chunk_data_size; // Use data_size for clarity

                // Allocate buffer for audio data
                buf = malloc(wav->data_size);
                if (buf == NULL) {
                    fprintf(stderr, "Error: Failed to allocate audio data buffer: %s\n", strerror(errno));
                    goto end;
                }

                // Copy audio data
                if (p - data + wav->data_size > len) {
                    fprintf(stderr, "Error: Data chunk size extends past end of file. Possible corruption.\n");
                    free(buf); // Free the buffer if partially allocated
                    goto end;
                }
                memcpy(buf, p, wav->data_size);
                SKIP_BYTES(wav->data_size);
                wav->data = buf; // Assign only after successful copy
                break;

            case 0x5453494C: /* 'LIST' */
                dprintf("id: LIST\n");
                SKIP_BYTES(chunk_data_size);
                break;

            case 0x20657563: /* 'cue ' */
                dprintf("id: cue \n");
                SKIP_BYTES(chunk_data_size);
                break;

            default:
                fprintf(stderr, "WARNING: unknown WAV chunk ID 0x%X in file %s at position: %ld (chunk size: %u)\n",
                        chunk_id, filename, (p - data) - 8, chunk_data_size); // Subtract 8 to show chunk ID start
                SKIP_BYTES(chunk_data_size); // Skip unknown chunk's data
                break;
        }

        /* make sure data buffer is word aligned - This should happen automatically when skipping chunks.
           If a chunk_data_size is odd, the next chunk starts on an even boundary by definition of RIFF.
           So after skipping `chunk_data_size` bytes, you effectively implicitly align.
           This explicit check here is mostly redundant unless you're reading byte by byte.
           It's safer to ensure the total bytes consumed by a chunk (including data + padding)
           is aligned before reading the *next* chunk header.
           A more robust way might be:
           If (chunk_data_size % 2 != 0) { SKIP_BYTES(1); }
           This would be after the chunk_data_size has been processed.
        */
        if ((p - data) % 2 == 1 && (p - data) < len) { // Only skip if not already at end
            dprintf("INFO: Skipping 1 byte for RIFF alignment.\n");
            SKIP_BYTES(1);
        }
    }

end:
    // This is the main cleanup point
    if (data != NULL) {
        free(data); // Free the full file buffer
        data = NULL;
    }
    // If wav was allocated but buf wasn't, or there was an error after buf alloc
    // but before assigning to wav->data, ensure buf is freed.
    // However, if wav->data is successfully assigned, wav_free handles it.
    // So, this is mostly covered.

    // If an error occurred before wav->data was assigned, buf might still hold memory
    // that needs to be freed if wav is returned as NULL.
    // Better to ensure wav_free gets called if wav object was partially built.
    if (wav != NULL && wav->data == NULL && buf != NULL) { // If buf was allocated but not assigned
        free(buf);
    }

    // If an error occurred (e.g., goto end was hit), wav might be partially filled
    // or invalid. Return NULL for error.
    if (wav == NULL || wav->data == NULL) { // If data chunk wasn't found or other critical error
        if (wav != NULL) { // If wav struct itself was allocated but not fully valid
            wav_free(wav); // Clean up any partial allocations in wav
        }
        return NULL;
    }

    return wav;
}

// Modify wav_free to be void, or clarify its purpose
void
wav_free(wav_t *wav)
{
    if (wav == NULL) return; // Defensive check

    if (wav->data != NULL) {
        free(wav->data);
        wav->data = NULL;
    }

    free(wav);
}

#ifdef WAV_MAIN
int
main(int argc, char **argv)
{
    wav_t *wav;
    char *progname = strrchr(argv[0], '/');
    if (!progname) { // Handle case where no / is present (e.g., Windows)
#ifdef _WIN32
        progname = strrchr(argv[0], '\\');
#endif
    }
    progname = progname ? progname + 1 : argv[0];
    int i;

    if (argc < 2) {
        fprintf(stderr, "usage: %s <filename> <...>\n", progname);
        exit(1);
    }

    for (i = 1; i < argc; i++) {
        wav = wav_new(argv[i]);

        if (wav == NULL) {
            fprintf(stderr, "error loading wav file: %s\n", argv[i]);
            // Don't exit, try next file
        } else {
            printf("%s: Channels: %u, Sample Rate: %uHz, Bits Per Sample: %u (%u bytes of PCM data)\n", argv[i],
                    wav->channels, wav->sample_rate, wav->bits_per_sample, wav->data_size);
            // Optionally print channel mask if present
            if (wav->channel_mask != 0) {
                 printf("  Channel Mask: 0x%X\n", wav->channel_mask);
            }
        }
        if (wav != NULL) { // Only free if successfully loaded
            wav_free(wav);
        }
    }

    return 0;
}
#endif
