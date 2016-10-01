/* Copyright 2014 Peter Román.
   This file is part of Flare which is licensed under GNU GPL v3.
   See the file named LICENSE for details. */

#include "chum.h"
#include "glenn.h"

int main(void)
{
        struct chum_context ctx;
        chum_init(&ctx);

        const float vs[] = {
                 0.0f,    0.5f,   0.0f, 1.0f,
                 0.5f,   -0.366f, 0.0f, 1.0f,
                -0.5f,   -0.366f, 0.0f, 1.0f
        };

        struct glenn_state s;
        glenn_init(&s, 3);
        glenn_update(&s, vs);
        glenn_resize(&s, 640, 480);

        while (chum_refresh(&ctx)) {
                int width;
                int height;

                chum_window_size(&ctx, &width, &height);
                glenn_resize(&s, width, height);
                glenn_display(&s);
        }

        chum_terminate(&ctx);
        return 0;
}
