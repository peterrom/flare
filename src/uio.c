/* Copyright 2014 Peter Román.
   This file is part of Flare which is licensed under GNU GPL v3.
   See the file named LICENSE for details. */

#include "uio.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct uio {
        char *mbuf;
        char *mbuf_end;
};

static bool mbuf_fits(struct uio *s, size_t byte_sz)
{
        return s->mbuf + byte_sz <= s->mbuf_end;
}

static void mbuf_push(struct uio *s, void *v, size_t byte_sz)
{
        memcpy(s->mbuf, v, byte_sz);
        s->mbuf += byte_sz;
}

static void mbuf_pop(struct uio *s, void *v, size_t byte_sz)
{
        memcpy(v, s->mbuf, byte_sz);
        s->mbuf += byte_sz;
}

struct uio *uio_open_mbuf(void *mbuf, size_t byte_sz)
{
        struct uio tmp = {
                .mbuf = mbuf,
                .mbuf_end = mbuf + byte_sz
        };

        struct uio *s = malloc(sizeof(struct uio));
        memcpy(s, &tmp, sizeof(*s));

        return s;
}

void uio_close(struct uio *s)
{
        free(s);
}

bool uio_put_i(struct uio *s, int v)
{
        if (!mbuf_fits(s, sizeof(v)))
                return false;

        mbuf_push(s, &v, sizeof(v));
        return true;
}

bool uio_get_i(struct uio *s, int *v)
{
        if (!mbuf_fits(s, sizeof(*v)))
                return false;

        mbuf_pop(s, v, sizeof(*v));
        return true;
}

bool uio_eof(struct uio *s)
{
        return s->mbuf == s->mbuf_end;
}
