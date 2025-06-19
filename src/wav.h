#ifndef _WAV_H
#define _WAV_H

typedef struct {
    uint16_t channels;
    uint32_t sample_rate;
    uint32_t byte_rate; // Renamed for clarity, often called byte_rate
    uint16_t block_align; // Renamed for clarity, often called block_align
    uint16_t bits_per_sample;
    uint32_t channel_mask; // Only for WAVE_FORMAT_EXTENSIBLE
    uint32_t data_size; // Renamed from size for clarity
    char *data;
} wav_t;

wav_t *wav_new(const char *);
void wav_free(wav_t *);

#endif
