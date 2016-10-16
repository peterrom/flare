#include <time.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "lime.h"

static void exit_with_errno(const char *msg)
{
        printf("%s: %s\n", msg, strerror(errno));
        exit(1);
}

static struct timespec timespec_sub(struct timespec a, struct timespec b)
{
        if (a.tv_nsec < b.tv_nsec) {
                return (struct timespec) {
                        .tv_sec = a.tv_sec - b.tv_sec - 1,
                        .tv_nsec = a.tv_nsec - b.tv_nsec + 1000000000
                };
        } else {
                return (struct timespec) {
                        .tv_sec = a.tv_sec - b.tv_sec,
                        .tv_nsec = a.tv_nsec - b.tv_nsec
               };
        }
}

static struct timespec timespec_add(struct timespec a, struct timespec b)
{
        struct timespec res = {
                .tv_sec = a.tv_sec + b.tv_sec,
                .tv_nsec = a.tv_nsec + b.tv_nsec
        };

        while (res.tv_nsec > 999999999) {
                res.tv_sec += 1;
                res.tv_nsec -= 999999999;
        }

        return res;
}

static bool timespec_gt(struct timespec a, struct timespec b)
{
        if (a.tv_sec > a.tv_sec)
                return true;

        if (a.tv_sec < a.tv_sec)
                return false;

        return a.tv_nsec > b.tv_nsec;
}

void lime_init(struct lime *s, unsigned int tick_time_ns) {
        struct timespec now;
        if (clock_gettime(CLOCK_MONOTONIC_RAW, &now) == -1)
                exit_with_errno("clock_gettime");

        struct timespec tick = {
                .tv_sec = tick_time_ns / 999999999,
                .tv_nsec = tick_time_ns % 999999999
        };

        *s = (struct lime) {
                .tick = tick,
                .target = timespec_add(now, tick)
        };
}

void lime_sleep(struct lime *s) {
        struct timespec now;
        if (clock_gettime(CLOCK_MONOTONIC_RAW, &now) == -1)
                exit_with_errno("clock_gettime");

        if (timespec_gt(s->target, now)) {
                const struct timespec remaining = timespec_sub(s->target, now);
                if (nanosleep(&remaining, NULL) == -1)
                        exit_with_errno("nanosleep");
        }

        s->target = timespec_add(s->target, s->tick);
}
