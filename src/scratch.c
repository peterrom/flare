/* Copyright 2014 Peter Román.
   This file is part of Flare which is licensed under GNU GPL v3.
   See the file named LICENSE for details. */

#include "scratch.h"

#include <string.h>
#include <assert.h>

#include "uio.h"

void scratch_init(struct scratch *s)
{
        s->valid_beg = s->buffer;
        s->valid_end = s->buffer;
}

static void flush_left(struct scratch *s)
{
        const size_t n = (size_t) (s->valid_end - s->valid_beg);
        memmove(s->buffer, s->valid_beg, n);

        s->valid_beg = s->buffer;
        s->valid_end = s->valid_beg + n;
}

static size_t tail_sz(const struct scratch *s)
{
        return (size_t) (s->buffer + sizeof(s->buffer) - s->valid_end);
}

static bool is_valid(const struct scratch *s)
{
        return s->valid_beg <= s->valid_end &&
                s->valid_beg >= s->buffer &&
                s->valid_end <= s->buffer + sizeof(s->buffer);
}

size_t scratch_fill(struct scratch *s, struct ui *is)
{
        assert(is_valid(s));

        flush_left(s);

        struct uo os;
        uo_buf(&os, s->valid_end, tail_sz(s));

        const size_t n = uio_copy(is, &os);
        s->valid_end += n;

        return n;
}

void scratch_clear(struct scratch *s)
{
        assert(is_valid(s));
        s->valid_end = s->valid_beg;
}

bool scratch_empty(const struct scratch *s)
{
        assert(is_valid(s));
        return s->valid_beg == s->valid_end;
}

bool scratch_full(const struct scratch *s)
{
        assert(is_valid(s));
        return s->valid_beg == s->buffer &&
                s->valid_end == s->buffer + sizeof(s->buffer);
}
