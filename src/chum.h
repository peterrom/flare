/* Copyright 2014 Peter Román.
   This file is part of Flare which is licensed under GNU GPL v3.
   See the file named LICENSE for details. */

#ifndef CHUM_INCL
#define CHUM_INCL

#include <stdbool.h>

/* * Platform Abstraction Layer (CHUM)

   CHUM is a reduced-functionality platform abstraction layer. Its
   main purpose is to provide an abstraction which should be easier to
   port to iOS/Android than GLFW. */

struct chum_context;

bool chum_init(struct chum_context *ctx);
void chum_terminate(struct chum_context *ctx);

bool chum_refresh(struct chum_context *ctx);

void chum_window_size(struct chum_context *ctx, int *width, int *height);

/* struct chum_context should be considered opaque. */
struct chum_context {
        void *internal;
};

#endif /* CHUM_INCL */
