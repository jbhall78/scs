#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "clock.h"
#include "timer.h"

// time is stored in nanoseconds

bool
sys_timer_is_ready(sys_timer_t *sys_timer)
{
    assert(sys_timer != NULL);
    assert(sys_timer->clock != NULL);

    clock_val_t now;
    clock_val_t delta;

    if (sys_timer->active == false)
	return false;

    sys_timer->clock->read(&now);
    sys_timer_delta_ts(&sys_timer->previous, &now, &delta);

/*
    printf("start: %d:%d  end: %d:%d   interval: %d:%d  delta: %d:%d\n",
	    sys_timer->previous.tv_sec, sys_timer->previous.tv_nsec, now.tv_sec, now.tv_nsec,
	    sys_timer->interval.tv_sec, sys_timer->interval.tv_nsec, delta.tv_sec, delta.tv_nsec);
*/
    if (
	    (delta.s >= sys_timer->interval.s) &&
	    (delta.ns >= sys_timer->interval.ns)
       ) {
	memcpy(&sys_timer->previous, &now, sizeof(clock_val_t));
	if (sys_timer->repeating == false) {
	    sys_timer->active = false;
	}
	return true;
    }

#if 0
    uint64_t ticks, off;

    if (sys_timer->active == false)
	return false;

    ticks = sys_timer->clock->ticks();
    off = ticks - sys_timer->previous;
	printf("grr: int%lld prev%lld ticks%lld off%lld\n", sys_timer->interval, sys_timer->previous, ticks, off);
    if (off >= sys_timer->interval) {
	sys_timer->previous = ticks;
	if (sys_timer->repeating == false) {
	    sys_timer->active = false;
	}
	return true;
    } else {
	return false;
    }
#endif
    return false;
}

void
sys_timer_stop(sys_timer_t *sys_timer)
{
    assert(sys_timer != NULL);

    sys_timer->active = false;
}

void
sys_timer_reset(sys_timer_t *sys_timer)
{
    assert(sys_timer != NULL);

    sys_timer->active = true;
//    sys_timer->previous = sys_timer->clock->ticks();
    sys_timer->clock->read(&sys_timer->previous);
}

void
sys_timer_set_repeating(sys_timer_t *sys_timer, bool repeating)
{
    assert(sys_timer != NULL);

    sys_timer->repeating = repeating;
}

// if tv_nsec > 1 second, add to seconds & reduce tv_nsec by that amount
void
sys_timer_normalize_ts(clock_val_t *ts)
{
    uint32_t amt;

//    printf("before: %d:%d\n", ts->s, ts->ns);
    // works with negative numbers

    //                   s      ms     us
    amt = ts->ns / (1000 * 1000 * 1000);
//    printf("scaling by: %d\n", amt);
    ts->s += amt;
    ts->ns -= amt * (1000 * 1000 * 1000);

    if (ts->ns < 0) {
	ts->s -= 1;
	ts->ns = (1000 * 1000 * 1000) - ts->ns;
    }
    assert(ts->s >= 0); // most likely programming error, but could be technically correct later on
    assert(ts->ns >= 0); // most likely programming error, but could be technically correct later on

//    printf("after: %d:%d\n", ts->s, ts->ns);
}

void
sys_timer_set_interval_ts(sys_timer_t *sys_timer, clock_val_t *ts)
{
    assert(sys_timer != NULL);
    assert(ts != NULL);

    memcpy(&sys_timer->interval, ts, sizeof(clock_val_t));
}

void
sys_timer_set_interval_ms(sys_timer_t *sys_timer, uint64_t interval)
{
    assert(sys_timer != NULL);

    sys_timer->interval.s  = 0;
    sys_timer->interval.ns = ms_to_ns(interval);
    sys_timer_normalize_ts(&sys_timer->interval);
}

void
sys_timer_set_interval_ns(sys_timer_t *sys_timer, uint64_t interval)
{
    assert(sys_timer != NULL);

    sys_timer->interval.s  = 0;
    sys_timer->interval.ns = interval;
    sys_timer_normalize_ts(&sys_timer->interval);
}

void
sys_timer_set_interval_us(sys_timer_t *sys_timer, uint64_t interval)
{
    assert(sys_timer != NULL);

    sys_timer->interval.s  = 0;
    sys_timer->interval.ns = us_to_ns(interval);
    sys_timer_normalize_ts(&sys_timer->interval);
}

void
sys_timer_sleep_ms(uint32_t ms)
{
    struct timeval tv;
    int ret;

    tv.tv_sec = 0;
    tv.tv_usec = ms_to_ns(ms);

    do {
	ret = select(0, NULL, NULL, NULL, &tv);
    } while (ret < 0);	// Linux specific
}

void
sys_timer_delta_ts(clock_val_t *start, clock_val_t *end, clock_val_t *res)
{
    res->s = end->s - start->s;
    res->ns = end->ns - start->ns;
    sys_timer_normalize_ts(res);
}

int64_t
sys_timer_delta_ns(clock_val_t *start, clock_val_t *end)
{
    int64_t res;
    res = s_to_ns(end->s - start->s);
    res += end->ns - start->ns;
    return res;
}

int64_t
sys_timer_delta_ms(clock_val_t *start, clock_val_t *end)
{
    int64_t res;
    res = s_to_ms(end->s - start->s);
    res += ns_to_ms(end->ns - start->ns);
    return res;
}

int64_t
sys_timer_delta_us(clock_val_t *start, clock_val_t *end)
{
    int64_t res;
    res = s_to_ns(end->s - start->s);
    res += ns_to_us(end->ns - start->ns);
    return res;
}

void
sys_timer_destroy(sys_timer_t *sys_timer)
{
    free(sys_timer);
}

sys_timer_t *
sys_timer_new(clock_driver_t *clock)
{
    sys_timer_t *sys_timer;
    sys_timer = malloc(sizeof(sys_timer_t));
    memset(sys_timer, 0, sizeof(sys_timer_t));

    sys_timer->clock = clock;

    return sys_timer;
}

#ifdef TIMER_MAIN
int
main(int argc, char **argv)
{
    clock_driver_t *drv = clock_init();
    sys_timer_t *timer;

    drv->open();

    timer = sys_timer_new(drv);
    sys_timer_set_interval_ms(timer, 200);
    sys_timer_set_repeating(timer, true);
    sys_timer_reset(timer);
    printf("start: (%lld)\n", drv->ticks());
    sleep(1);
    printf("start: (%lld)\n", drv->ticks());

    while (1) {
	if (sys_timer_is_ready(timer)) {
	    printf("b00m! (%lld)\n", drv->ticks());
	}
    }
 
    return 0;
}
#endif
