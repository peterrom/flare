/* Copyright 2014 Peter Román.
   This file is part of Flare which is licensed under GNU GPL v3.
   See the file named LICENSE for details. */

#ifndef CHUM_INCL
#define CHUM_INCL

#include <stdbool.h>

/* * Platform Abstraction Layer (CHUM)
 */

struct chum_context {
        void *internal;
};

bool chum_init(struct chum_context *ctx);
void chum_terminate(struct chum_context *ctx);

bool chum_refresh(struct chum_context *ctx);

#endif /* CHUM_INCL */
