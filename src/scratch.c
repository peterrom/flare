/* Copyright 2014 Peter Román.
   This file is part of Flare which is licensed under GNU GPL v3.
   See the file named LICENSE for details. */

#include "scratch.h"

#include <string.h>
#include <assert.h>

void scratch_init(struct scratch *s)
{
        s->valid_beg = s->buffer;
        s->valid_end = s->buffer;
}

void scratch_flush_left(struct scratch *s)
{
        assert(s->valid_end >= s->valid_beg);

        const size_t n = s->valid_end - s->valid_beg;
        memmove(s->buffer, s->valid_beg, n);

        s->valid_beg = s->buffer;
        s->valid_end = s->valid_beg + n;
}

static size_t tail_sz(const struct scratch *s)
{
        return s->buffer + sizeof(s->buffer) - s->valid_end;
}

void scratch_fill(struct scratch *s, struct uio *is)
{
        struct uio os = uio_mbuf(s->valid_end, tail_sz(s));
        s->valid_end += uio_copy(is, &os);
}

static bool is_any_of(char c, const char *characters)
{
        for (; *characters != '\0'; characters++)
                if (c == *characters)
                        return true;

        return false;
}

char *scratch_find_any_of(const struct scratch *s, const char *characters)
{
        for (char *i = s->valid_beg; i != s->valid_end; ++i)
                if (is_any_of(*i, characters))
                        return i;

        return NULL;
}
