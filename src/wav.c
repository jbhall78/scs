#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "wav.h"

/* enable untested code? */
#define UNTESTED

/* enable debug output? */
#undef DEBUG

#ifdef DEBUG
#define dprintf(...) printf(__VA_ARGS__)
#else
#define dprintf(...)
#endif


#define read16(a)	memcpy(a, p, sizeof(uint16_t)); \
			p += sizeof(uint16_t);

#define read32(a)	memcpy(a, p, sizeof(uint32_t)); \
			p += sizeof(uint32_t);

#define skip(bytes)	p += bytes;


static int
read_file(char *filename, void **_data, uint64_t *_len)
{
    int fd;
    struct stat st;
    uint64_t len;
    char *data;

    /* JBH - this hasn't been ported to windows yet */

    /* get file size */
    if (stat(filename, &st) < 0) {
	fprintf(stderr, "cannot stat wav file: %s: %s\n",
		filename, strerror(errno));
	return -1;
    }
    len = st.st_size;

    /* open file */
    if ((fd = open(filename, O_RDONLY)) < 0) {
	fprintf(stderr, "error opening wav file: %s: %s\n",
		filename, strerror(errno));
	return -1;
    }

    /* allocate space */
    data = malloc(len);
    if (data == NULL)
	return -1;
    memset(data, 0, len);

    /* read file */
    if (read(fd, data, len) < 0) {
	fprintf(stderr, "cannot read file: %s: %s\n",
		filename, strerror(errno));
	return -1;
    }

    /* close file */
    close(fd);

    *_len = len;
    *_data = data;

    return 1;
}


wav_t *
wav_new(char *filename)
{
    wav_t *wav = NULL;
    char *buf = NULL, *data = NULL, *p = NULL;
    uint64_t len = 0;
    uint32_t n32;
    uint16_t n16;
    uint32_t guid[4];
    uint16_t chunk_size;

    /* read file into memory */
    if (read_file(filename, (void **)&data, &len) < 0)
	goto end;

    /* allocate object */
    wav = malloc(sizeof(wav_t));
    if (wav == NULL)
	goto end;
    memset(wav, 0, sizeof(wav_t));

    for (p = data; p - data < len; ) {
	read32(&n32);

	switch(n32) {
	    case 0x45564157: /* WAVE */
		dprintf("id: WAVE\n");
		break;

	    case 0x46464952: /* RIFF */
		dprintf("id: RIFF\n");

		read32(&n32);
		dprintf("size: %d\n", n32);
		break;
	    case 0x20746D66: /* fmt  */
		dprintf("id: fmt\n");

		read32(&n32);
		dprintf("size: %d\n", n32);
		chunk_size = n32;

		/* read format code */
		read16(&n16);
		dprintf("format: %d\n", n16);

		/* read channels */
		read16(&n16);
		dprintf("channels: %d\n", n16);
		wav->channels = n16;

		/* read sample rate */
		read32(&n32);
		dprintf("sample rate: %d\n", n32);
		wav->sample_rate = n32;

		/* data rate */
		read32(&n32);
		dprintf("data rate: %d\n", n32);
		wav->data_rate = n32;

		/* data block size */
		read16(&n16);
		dprintf("data block size: %d\n", n16);
		wav->data_block_size = n16;

		/* bits per sample */
		read16(&n16);
		dprintf("bits per sample: %d\n", n16);
		wav->bits_per_sample = n16;

#ifdef UNTESTED /* untested code */
		if (chunk_size > 16) {
		    /* cbSize (extension size) (0 || 22) */
		    read16(&n16);
		    dprintf("cbSize: %d\n", n16);
		}

		if (chunk_size == 40 && n16 == 22) {
		    /* wBitsPerSample (valid bits per sample) ??? */
		    read16(&n16);
		    dprintf("valid bits per sample: %d\n", n16);

		    /* channel mask */
		    read32(&n32);
		    dprintf("channel mask: %d\n", n32);

		    /* sub format */
		    memcpy(guid, p, sizeof(guid));
		    p += sizeof(guid);
		}
#endif /* UNTESTED */
		break;

	    case 0x74636166: /* fact */
		dprintf("id: fact\n");
		break;

	    case 0x61746164: /* data */
		dprintf("id: data\n");
		read32(&n32);
		dprintf("size: %d\n", n32);
		wav->size = n32;

		buf = malloc(n32);
		if (buf == NULL) goto end;

		memcpy(buf, p, n32);
		p += n32;
		wav->data = buf;

		break;

	    case 0x5453494C: /* LIST */
		dprintf("id: LIST\n");

		read32(&n32);
		dprintf("size: %d\n", n32);

		skip(n32);
		break;

	    case 0x20657563: /* cue  */
		dprintf("id: cue\n");

		read32(&n32);
		dprintf("size: %d\n", n32);

		skip(n32);
		break;

	    default:
		fprintf(stderr, "WARNING: unknown WAV data %d in file %s at position: %ld\n", n32, filename, p - data);
		dprintf("id: %d\n", n32);
		break;
	}

	/* make sure data buffer is word aligned */
	if ((p - data) % 2 == 1)
	    skip(1);
    }

end:

    free(data);

    return wav;
}

wav_t *
wav_free(wav_t *wav)
{
    if (wav->data != NULL)
	free(wav->data);

    free(wav);

    return NULL;
}

#ifdef WAV_MAIN
int
main(int argc, char **argv)
{
    wav_t *wav;
    char *progname = strrchr(argv[0], '/');
    progname = progname ? progname : argv[0];
    int i;

    if (argc < 2) {
	fprintf(stderr, "usage: %s <filename> <...>\n", progname);
	exit(1);
    }

    for (i = 1; i < argc; i++) {
	wav = wav_new(argv[i]);

	if (wav == NULL) {
	    fprintf(stderr, "error loading wav file: %s\n", argv[i]);
	} else {
	    printf("%s: %s %dbit %dHz (%d bytes of PCM data)\n", argv[i],
		    (wav->channels == 1) ? "Mono" : "Stereo",
		    wav->bits_per_sample, wav->sample_rate, wav->size);
	}
	wav_free(wav);
    }

    return 0;
}
#endif
