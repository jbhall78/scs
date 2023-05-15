#ifndef TIMER_H
#define TIMER_H

#include <stdbool.h>

#include "clock.h"

typedef struct sys_timer_s {
    clock_driver_t *clock;	// clock driver
//    uint64_t interval;		// interval in milliseconds
    bool repeating;		// repeating?
    bool active;
//    uint64_t previous;	// previous alarm time

    // time is stored in microseconds
    clock_val_t previous;
    clock_val_t interval;
} sys_timer_t;

#define sys_timer_start sys_timer_reset

bool sys_timer_is_ready(sys_timer_t *sys_timer);
void sys_timer_stop(sys_timer_t *sys_timer);
void sys_timer_reset(sys_timer_t *sys_timer);
void sys_timer_set_repeating(sys_timer_t *sys_timer, bool repeating);
void sys_timer_set_interval(sys_timer_t *sys_timer, uint64_t interval);
void sys_timer_destroy(sys_timer_t *sys_timer);
sys_timer_t *sys_timer_new(clock_driver_t *clock);

void sys_timer_set_interval_ms(sys_timer_t *sys_timer, uint64_t interval);
void sys_timer_set_interval_ns(sys_timer_t *sys_timer, uint64_t interval);
void sys_timer_set_interval_us(sys_timer_t *sys_timer, uint64_t interval);

/* this stuff should probably go somewhere else,
 * but will keep it here for now until I determine
 * how to name everything
 */
void sys_timer_sleep_ms(uint32_t ms);

void sys_timer_delta_ts(clock_val_t *start, clock_val_t *end, clock_val_t *res);
int64_t sys_timer_delta_ns(clock_val_t *start, clock_val_t *end);
int64_t sys_timer_delta_ms(clock_val_t *start, clock_val_t *end);
int64_t sys_timer_delta_us(clock_val_t *start, clock_val_t *end);

#endif
