#ifndef _CLOCK_H
#define _CLOCK_H

#include <stdint.h>
#include <time.h>
#include <sys/time.h>

typedef struct {
    uint32_t s;		// seconds
    uint32_t ns;	// nanoseconds
} clock_val_t;

typedef struct {
    void (*open)(void);
    // drivers are expected to convert their values into microseconds
    void (*read)(clock_val_t *cv);
    void (*close)(void);
    uint64_t (*ticks)(void);
} clock_driver_t;

clock_driver_t *clock_driver;
extern clock_driver_t clock_gettimeofday_driver;
extern clock_driver_t clock_gettime_driver;

// us = microsecond
// ms = millisecond
// ns = nanosecond
#define s_to_ms(val)	((val) * 1000)
#define s_to_us(val)	((val) * 1000 * 1000)
#define s_to_ns(val)	((val) * 1000 * 1000 * 1000)

#define ms_to_ns(val)	((val) * 1000 * 1000)
#define ms_to_us(val)	((val) * 1000)
#define ms_to_s(val)	((val) / 1000)

#define us_to_ns(val)	((val) * 1000)
#define us_to_ms(val)	((val) / 1000)
#define us_to_s(val)	((val) / 1000 / 1000)

#define ns_to_ms(val)	((val) / 1000 / 1000)
#define ns_to_us(val)	((val) / 1000)
#define ns_to_s(val)	((val) / 1000 / 1000 / 1000)

clock_driver_t *clock_init(void);

#if 0
/*
 * gettimeofday time driver
 */
void clock_gettimeofday_read(struct timeval *tv);

/*
 * clock_gettime time driver
 */
void clock_gettime_open(void);
void clock_gettime_read(struct timeval *tv);
#endif

#endif
