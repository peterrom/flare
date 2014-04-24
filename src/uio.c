/* Copyright 2014 Peter Román.
   This file is part of Flare which is licensed under GNU GPL v3.
   See the file named LICENSE for details. */

#include "uio.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct uio uio_null()
{
        return (struct uio) { 0 };
}

static bool mbuf_fits(struct uio *s, size_t byte_sz)
{
        return s->mbuf + byte_sz <= s->mbuf_end;
}

static void mbuf_push(struct uio *s, void *v, size_t byte_sz)
{
        memcpy(s->mbuf, v, byte_sz);
        s->mbuf += byte_sz;
}

static void mbuf_peek(struct uio *s, void *v, size_t byte_sz)
{
        memcpy(v, s->mbuf, byte_sz);
}

static void mbuf_pop(struct uio *s, size_t byte_sz)
{
        s->mbuf += byte_sz;
}

struct uio uio_mbuf(void *mbuf, size_t byte_sz)
{
        struct uio tmp = {
                .mbuf = mbuf,
                .mbuf_end = (char *) mbuf + byte_sz
        };

        return tmp;
}

struct uio uio_mbuf_range(void *beg, void *end)
{
        struct uio tmp = {
                .mbuf = beg,
                .mbuf_end = end
        };

        return tmp;
}

void uio_close(struct uio *s)
{
}

bool uio_eof(struct uio *s)
{
        return s->mbuf == s->mbuf_end;
}

size_t uio_copy(struct uio *src, struct uio *dst)
{
        size_t i = 0;

        for (;; ++i) {
                char tmp;

                if (!uio_peek_c(src, &tmp) || !uio_put_c(dst, tmp))
                        break;

                uio_get_c(src, NULL);
        }

        return i;
}

size_t uio_copy_n(struct uio *src, struct uio *dst, size_t n)
{
        for (size_t i = 0; i < n; ++i) {
                char tmp;

                if (!uio_peek_c(src, &tmp) || !uio_put_c(dst, tmp))
                        return i;

                uio_get_c(src, NULL);
        }

        return n;
}

bool uio_find(struct uio *s, const void *pattern, size_t byte_sz)
{
        const char *const p = pattern;
        size_t i = 0;

        char tmp;

        while (uio_get_c(s, &tmp)) {
                if (tmp == p[i]) {
                        ++i;

                        if (i == byte_sz)
                                return true;
                } else {
                        i = 0;
                }
        }

        return false;
}

#define DEF_PUT_GET_PEEK_FOR(TYPE, SUFF)                \
        bool uio_put_ ## SUFF(struct uio *s, TYPE v)    \
        {                                               \
                if (!mbuf_fits(s, sizeof(v)))           \
                        return false;                   \
                                                        \
                mbuf_push(s, &v, sizeof(v));            \
                return true;                            \
        }                                               \
                                                        \
        bool uio_get_ ## SUFF(struct uio *s, TYPE *v)   \
        {                                               \
                if (!mbuf_fits(s, sizeof(*v)))          \
                        return false;                   \
                                                        \
                if (v)                                  \
                        mbuf_peek(s, v, sizeof(*v));    \
                                                        \
                mbuf_pop(s, sizeof(*v));                \
                return true;                            \
        }                                               \
                                                        \
        bool uio_peek_ ## SUFF(struct uio *s, TYPE *v)  \
        {                                               \
                if (!mbuf_fits(s, sizeof(*v)))          \
                        return false;                   \
                                                        \
                mbuf_peek(s, v, sizeof(*v));            \
                return true;                            \
        }

DEF_PUT_GET_PEEK_FOR(int, i)
DEF_PUT_GET_PEEK_FOR(char, c)
