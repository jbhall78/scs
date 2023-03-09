#ifndef RANDOM_H
#define RANDOM_H

#define RANDOM_DEV_FAST	(1 << 0)
#define RANDOM_DEV_SLOW	(1 << 1)

struct rnd_driver_s {
	void (*open)(int hints);
	int32_t (*number)(int32_t min, int32_t max);
	void (*bytes)(int32_t num, int8_t *buf);
	void (*close)(void);
	void (*seed)(void);
};

typedef struct rnd_driver_s rnd_driver_t;

// drivers
extern rnd_driver_t rnd_driver_rand;
extern rnd_driver_t rnd_driver_random;
extern rnd_driver_t rnd_driver_dev_random;
extern rnd_driver_t rnd_driver_win32_crypt;

// exported api
void rnd_open(int hints);
void rnd_close(void);
void rnd_bytes(int num, int8_t *buf);
int32_t rnd_number(int min, int max);
void rnd_seed(void);

#endif
