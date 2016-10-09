/* Copyright 2014, 2016 Peter Román.
   This file is part of Flare which is licensed under GNU GPL v3.
   See the file named LICENSE for details. */

#include "chum.h"
#include "glenn.h"
#include "sec.h"

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

        while (chum_refresh(&ctx)) {
                int width;
                int height;

                chum_window_size(&ctx, &width, &height);
                glenn_resize(&s, width, height);
                glenn_update(&s, (const float *) w.triangle[i].vx);
                glenn_display(&s);
        }

        chum_terminate(&ctx);
        return 0;
}
