/* Copyright 2014 Peter Román.
   This file is part of Flare which is licensed under GNU GPL v3.
   See the file named LICENSE for details. */

#ifndef SCRATCH_INCL
#define SCRATCH_INCL

/* * Scratch space (SCRATCH)
   SCRATCH is designed for processing data from a UIO stream in
   chunks. It allows you to easily save parts of a chunk by setting
   the ~scratch.valid_*~ pointers, i.e.

   : struct scratch ss;
   : scratch_init(&ss);

   ... set the buffer to the string ~"01234"~ ...

   : ss.valid_beg += 3;
   : scratch_flush_left(&ss);

   This will result in the buffer containing the string ~"34"~. You
   can then fill the remaining chunk with

   : scratch_fill_end(&ss, &is);

   The buffer now contains ~{ '3', '4', '\0' }~ and then ~"567" ...~
   or whatever is in the stream ~is~. */

#include <stdbool.h>

struct uio;

struct scratch {
        char buffer[32];

        char *valid_beg;
        char *valid_end;
};

void scratch_init(struct scratch *s);

/* Move the valid region to the beginning of the buffer. */
void scratch_flush_left(struct scratch *s);

/* Fill the region from ~valid_end~ to the end of the buffer from ~is~. */
void scratch_fill_end(struct scratch *s, struct uio *is);

/* Open a UIO stream to the valid region of the buffer. */
struct uio scratch_valid(struct scratch *s);

bool scratch_empty(struct scratch *s);

#endif /* SCRATCH_INCL */
