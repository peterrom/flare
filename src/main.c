/* Copyright 2014, 2016 Peter Román.
   This file is part of Flare which is licensed under GNU GPL v3.
   See the file named LICENSE for details. */

#include <time.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "chum.h"
#include "glenn.h"
#include "sec.h"

void exit_with_errno(void)
{
        printf("%s\n", strerror(errno));
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


int main(void)
{
        struct sec_world w;
        sec_world_init(&w);

        const int i = sec_new(&w);
        w.component[i] = sec_c_triangle;
        w.triangle[i] = (struct sec_triangle) {
                .vx = {{  0.0f,  0.5f,   0.0f, 1.0f },
                       {  0.5f, -0.366f, 0.0f, 1.0f },
                       { -0.5f, -0.366f, 0.0f, 1.0f }}};

        struct chum_context ctx;
        chum_init(&ctx);

        struct glenn_state s;
        glenn_init(&s, 3);
        glenn_resize(&s, 640, 480);

        const struct timespec dt = {
                .tv_sec = 0,
                .tv_nsec = 16666667
        };

        struct timespec now;

        if (clock_gettime(CLOCK_MONOTONIC_RAW, &now) == -1)
                exit_with_errno();

        struct timespec target = timespec_add(now, dt);

        while (chum_refresh(&ctx)) {
                int width;
                int height;

                chum_window_size(&ctx, &width, &height);
                glenn_resize(&s, width, height);
                glenn_update(&s, (const float *) w.triangle[i].vx);
                glenn_display(&s);

                if (clock_gettime(CLOCK_MONOTONIC_RAW, &now) == -1)
                        exit_with_errno();

                if (timespec_gt(target, now)) {
                        const struct timespec remaining = timespec_sub(target, now);
                        if (nanosleep(&remaining, NULL) == -1)
                                exit_with_errno();
                }

                target = timespec_add(target, dt);
        }

        chum_terminate(&ctx);
        return 0;
}
