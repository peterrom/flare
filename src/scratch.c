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

void scratch_fill_end(struct scratch *s, struct uio *is)
{
        struct uio os = uio_mbuf(s->valid_end, tail_sz(s));
        s->valid_end += uio_copy(is, &os);
}

struct uio scratch_valid(struct scratch *s)
{
        return uio_mbuf_range(s->valid_beg, s->valid_end);
}

bool scratch_empty(struct scratch *s)
{
        return s->valid_beg == s->valid_end;
}
