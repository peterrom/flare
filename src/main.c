/* Copyright 2014 Peter Román.
   This file is part of Flare which is licensed under GNU GPL v3.
   See the file named LICENSE for details. */

#include "chum.h"
#include "glenn.h"

int main(void)
{
        struct chum_context ctx;
        chum_init(&ctx);

        struct glenn_state s;
        glenn_init(&s);
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
