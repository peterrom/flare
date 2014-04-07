/* Copyright 2014 Peter Román.
   This file is part of Flare which is licensed under GNU GPL v3.
   See the file named LICENSE for details. */

#ifndef FORTY_INCL
#define FORTY_INCL

/* * FORmatted TYper (FORTY)
 */

#include "uio.h"

struct forty_action {
        const char *id;
        void (*push) (struct uio *arg_s);
        void (*pop) (void);
};

/* Interpret the data from /s/ according to /actions/.
   Return /true/ if successful. */
bool forty_print(const struct forty_action *actions, struct uio *s);

#endif /* FORTY_INCL */
