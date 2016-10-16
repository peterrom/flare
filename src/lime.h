/* Copyright 2016 Peter Román.
   This file is part of Flare which is licensed under GNU GPL v3.
   See the file named LICENSE for details. */

#ifndef LIME_INCL
#define LIME_INCL

/* * Light tIME platform abstraction layer (LIME)
 */

#include <time.h>

struct lime;

void lime_init(struct lime *s, unsigned int tick_time_ns);
void lime_sleep(struct lime *s);

/* struct lime should be considered opaque */
struct lime {
        struct timespec tick;
        struct timespec target;
};

#endif /* LIME_INCL */
