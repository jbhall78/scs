#ifndef _WAV_H
#define _WAV_H

typedef struct {
    uint16_t channels;
    uint32_t sample_rate;
    uint32_t data_rate;
    uint16_t data_block_size;
    uint16_t bits_per_sample;
    uint32_t channel_mask;
    uint32_t size;
    char *data;
} wav_t;

wav_t *wav_new(char *);
wav_t *wav_free(wav_t *);

#endif
