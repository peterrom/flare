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

   : ss.valid_beg = scratch_find_any_of(&ss, "3");
   : scratch_flush_left(&ss);

   This will result in the buffer containing the string ~"34"~. You
   can then fill the remaining chunk with

   : scratch_fill_end(&ss, &is);

   The buffer now contains ~{ '3', '4', '\0' }~ and then ~"567" ...~
   or whatever is in the stream ~is~. */

struct uio;

struct scratch {
        char buffer[32];

        char *valid_beg;
        char *valid_end;
};

void scratch_init(struct scratch *s);

/* Move the valid region to the beginning of the buffer. */
void scratch_flush_left(struct scratch *s);

/* Fill the region from /valid_end/ to the end of the buffer from /is/. */
void scratch_fill_end(struct scratch *s, struct uio *is);

/* Find a character in the scratch space. Return the position or NULL. */
char *scratch_find_any_of(const struct scratch *s, const char *characters);

#endif /* SCRATCH_INCL */
