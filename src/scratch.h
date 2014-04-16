/* Copyright 2014 Peter Román.
   This file is part of Flare which is licensed under GNU GPL v3.
   See the file named LICENSE for details. */

#ifndef SCRATCH_INCL
#define SCRATCH_INCL

/* * Scratch space (SCRATCH)
   SCRATCH is designed for processing data from a UIO stream in
   chunks. It allows you to easily save parts of a chunk by setting
   the ~scratch.valid_*~ pointers, i.e.

   : struct uio is = ...; // feeds ascending numbers
   :
   : struct scratch ss;
   : scratch_init(&ss);
   :
   : scratch_fill(&ss, &is);

   If the scratch buffer had a size of 6 it would now contain ~"012345"~.

   : ss.valid_beg += 2;
   : ss.valid_end -= 2;
   :
   : scratch_fill(&ss);

   This will result in the buffer containing the string ~"236789"~. */

#include <stdbool.h>
#include <stddef.h>

struct uio;

struct scratch {
        char buffer[32];

        char *valid_beg;
        char *valid_end;
};

void scratch_init(struct scratch *s);

/* Move the valid region of ~s~ to the beginning of the buffer and
   fill the region between ~s->valid_end~ and the end of the buffer
   with data from ~is~.

   Return the number of bytes added to ~s~. */
size_t scratch_fill(struct scratch *s, struct uio *is);

/* Clear the scratch buffer (from valid data). */
void scratch_clear(struct scratch *s);

/* Open a UIO stream to the valid region of the buffer. */
struct uio scratch_valid(struct scratch *s);

/* Return ~true~ if ~*s~ is empty. */
bool scratch_empty(const struct scratch *s);

/* Return ~true~ if ~*s~ is full, i.e. a call to ~scratch_fill~
   would return ~0~. */
bool scratch_full(const struct scratch *s);

#endif /* SCRATCH_INCL */
