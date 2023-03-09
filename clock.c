#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>

#include "clock.h"

clock_driver_t  *clock_driver;

static clock_val_t start;

#define SECONDS_TO_EPOCH 11644473600LL

/////////////////////////////////////////////////////////////////
#ifdef WIN32
#include <windows.h>

void
clock_getsystemtime_read(clock_val_t *cv)
{
    ULARGE_INTEGER ft;
    GetSystemTimeAsFileTime(ft);
    cv->s = us_to_s(ft.QuadPart) / 10 - SECONDS_TO_EPOCH;
    cv->ns = ft.QuadPart - (s_to_us(cv->s) * 10);
    cv->ns = us_to_ns(cv->ns * 100)
}

void
clock_getsystemtime_close(void)
{

}
void
clock_getsystemtime_open(void)
{
    clock_getsystemtime_read(&start);
}

uint64_t
clock_getsystemtime_ticks(void)
{
    uint64_t ticks, ticks1, ticks2;
    clock_val_t now;

    clock_getsystemtime_read(&now);

    ticks1 = s_to_ms(start.s);
    ticks1 += ns_to_ms(start.ns);

    ticks2 = s_to_ms(now.s);
    ticks2 += ns_to_ms(now.ns);

    ticks = ticks2 - ticks1;

    return ticks;
}

clock_driver_t clock_getsystemtime_driver = {
    .open  = clock_getsystemtime_open,
    .read  = clock_getsystemtime_read,
    .close = clock_getsystemtime_close,
    .ticks = clock_getsystemtime_ticks,
};

/////////////////////////////////////////////////////////////////
#elif LINUX

/*
 * clock_gettime time driver
 */
void
clock_gettime_read(clock_val_t *cv)
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    cv->s  = ts.tv_sec;
    cv->ns = ts.tv_nsec;
}

void
clock_gettime_close(void)
{
    return;
}

void
clock_gettime_open(void)
{
    struct timespec ts;
    clock_getres(CLOCK_REALTIME, &ts);
    printf("clock resolution: %ld nanoseconds\n", ts.tv_nsec);
    clock_gettime_read(&start);
}

uint64_t
clock_gettime_ticks(void)
{
    uint64_t ticks, ticks1, ticks2;
    clock_val_t now;

    clock_gettime_read(&now);

    ticks1 = s_to_ms(start.s);
    ticks1 += ns_to_ms(start.ns);

    ticks2 = s_to_ms(now.s);
    ticks2 += ns_to_ms(now.ns);

    ticks = ticks2 - ticks1;

//    printf(">>>> %d:%d %d:%d %lld\n", start.s, start.ns, now.s, now.ns, ticks);
    return ticks;
}

clock_driver_t clock_gettime_driver = {
    .open  = clock_gettime_open,
    .read  = clock_gettime_read,
    .close = clock_gettime_close,
    .ticks = clock_gettime_ticks,
};

/////////////////////////////////////////////////////////////////

#elif UNIX

/*
 * gettimeofday time driver
 */
void
clock_gettimeofday_read(clock_val_t *cv)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    cv->s  = tv.tv_sec;
    cv->ns = us_to_ns(tv.tv_usec);
}

void
clock_gettimeofday_close(void)
{
    return;
}

void
clock_gettimeofday_open(void)
{
    clock_gettimeofday_read(&start);
}

uint64_t
clock_gettimeofday_ticks(void)
{
    uint64_t ticks, ticks1, ticks2;
    clock_val_t now;

    clock_gettimeofday_read(&now);

/*
    ticks = s_to_ms(now.s - start.s);
    ticks += ns_to_ms(now.ns - start.ns);
*/

    ticks1 = s_to_ms(start.s);
    ticks1 += ns_to_ms(start.ns);

    ticks2 = s_to_ms(now.s);
    ticks2 += ns_to_ms(now.ns);

    ticks = ticks2 - ticks1;

    return ticks;
}

clock_driver_t clock_gettimeofday_driver = {
    .open  = clock_gettimeofday_open,
    .read  = clock_gettimeofday_read,
    .close = clock_gettimeofday_close,
    .ticks = clock_gettimeofday_ticks,
};
#endif

clock_driver_t *
clock_init(void)
{
#ifdef LINUX
    clock_driver_t *drv = &clock_gettime_driver;
#elif UNIX
    clock_driver_t *drv = &clock_gettimeofday_driver;
#elif WIN32
    clock_driver_t *drv = &clock_getsystemtime_driver;
#else
#error No Clock Driver!
#endif

    return drv;
}

#ifdef CLOCK_MAIN
int
main(int argc, char **argv)
{
    int i;

    clock_driver_t *drv = clock_init();

    drv->open();

    for (i = 0; i < 5; i++) {
	int64_t ticks = drv->ticks();
	printf("%lld\n", ticks);
	sleep(1);
    }
    return 0;
}
#endif
