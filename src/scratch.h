/* Copyright 2014 Peter Román.
   This file is part of Flare which is licensed under GNU GPL v3.
   See the file named LICENSE for details. */

#ifndef SCRATCH_INCL
#define SCRATCH_INCL

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
