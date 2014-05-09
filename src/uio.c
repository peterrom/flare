/* Copyright 2014 Peter Román.
   This file is part of Flare which is licensed under GNU GPL v3.
   See the file named LICENSE for details. */

#include "uio.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

static size_t min_sz(size_t a, size_t b)
{
        return a < b ? a : b;
}

static size_t range_sz(const char *beg, const char *end)
{
        assert(beg <= end);
        return (size_t) (end - beg);
}

/* ~cb~ is a simple static circular buffer implementation used for
   buffering reads/writes on the stack. */

static size_t cb_idx(size_t n)
{
        return n & 0x3f;
}

static size_t cb_available(struct uio_cb *cb)
{
        assert(cb->w >= cb->r);
        return (size_t) (cb->w - cb->r);
}

static size_t cb_empty(struct uio_cb *cb)
{
        assert(sizeof(cb->buf) >= cb_available(cb));
        return (size_t) (sizeof(cb->buf) - cb_available(cb));
}

static bool cb_push(struct uio_cb *cb, const void *v, size_t byte_sz)
{
        if (cb_empty(cb) < byte_sz)
                return false;

        const char *cv = v;

        for (size_t i = 0; i < byte_sz; ++i) {
                cb->buf[cb_idx(cb->w)] = cv[i];
                ++cb->w;
        }

        return true;
}

static bool cb_peek(struct uio_cb *cb, void *v, size_t byte_sz)
{
        if (cb_available(cb) < byte_sz)
                return false;

        char *cv = v;

        for (size_t i = 0; i < byte_sz; ++i)
                cv[i] = cb->buf[cb_idx(cb->r + i)];

        return true;
}

static bool cb_pop(struct uio_cb *cb, size_t byte_sz)
{
        if (cb_available(cb) < byte_sz)
                return false;

        cb->r += byte_sz;

        return true;
}

/* ~raw~ are os/target dependent raw i/o methods. */

static size_t raw_read_buf(const char **buf_pos, const char *buf_end,
                           void *buf_out, size_t byte_sz)
{
        const size_t n = min_sz(range_sz(*buf_pos, buf_end), byte_sz);
        memcpy(buf_out, *buf_pos, n);

        *buf_pos += n;
        return n;
}

static size_t raw_write_buf(char **buf_pos, char *buf_end,
                            void *buf, size_t byte_sz)
{
        const size_t n = min_sz(range_sz(*buf_pos, buf_end), byte_sz);
        memcpy(*buf_pos, buf, n);

        *buf_pos += n;
        return n;
}

static size_t raw_read(struct ui *is, void *buf, size_t byte_sz)
{
        return raw_read_buf(&is->buf_pos, is->buf_end, buf, byte_sz);
}

static size_t raw_write(struct uo *os, void *buf, size_t byte_sz)
{
        return raw_write_buf(&os->buf_pos, os->buf_end, buf, byte_sz);
}

/* The next internal layer of UIO are the buffered input/output
   methods. */

static void ui_flush(struct ui *is)
{
        char tmpbuf[sizeof(is->cb.buf)];
        const size_t n = raw_read(is, tmpbuf, cb_empty(&is->cb));

        cb_push(&is->cb, tmpbuf, n);
}

static bool bio_peek(struct ui *is, void *buf, size_t byte_sz)
{
        if (cb_peek(&is->cb, buf, byte_sz))
                return true;

        ui_flush(is);

        return cb_peek(&is->cb, buf, byte_sz);
}

static bool bio_pop(struct ui *is, size_t byte_sz)
{
        if (cb_pop(&is->cb, byte_sz))
                return true;

        ui_flush(is);

        return cb_pop(&is->cb, byte_sz);
}

static bool bio_write(struct uo *os, const void *buf, size_t byte_sz)
{
        if (cb_push(&os->cb, buf, byte_sz))
                return true;

        uo_flush(os);

        return cb_push(&os->cb, buf, byte_sz);
}

/* With these building blocks defined, we can implement the public
   interface of UIO. */

bool ui_null(struct ui *is)
{
        if (!is)
                return false;

        *is = (struct ui) { 0 };
        return true;
}

bool uo_null(struct uo *os)
{
        if (!os)
                return false;

        *os = (struct uo) { 0 };
        return true;
}

bool ui_buf(struct ui *is, const void *buf, size_t byte_sz)
{
        if (!is || !buf)
                return false;

        *is = (struct ui) {
                .buf_pos = buf,
                .buf_end = (const char *) buf + byte_sz,
                .cb = { 0 }
        };

        return true;
}

bool uo_buf(struct uo *os, void *buf, size_t byte_sz)
{
        if (!os || !buf)
                return false;

        *os = (struct uo) {
                .buf_pos = buf,
                .buf_end = (char *) buf + byte_sz,
                .cb = { 0 }
        };

        return true;
}

void ui_close(struct ui *is)
{
}

void uo_close(struct uo *os)
{
        uo_flush(os);
}

size_t uo_flush(struct uo *os)
{
        char tmpbuf[sizeof(os->cb.buf)];
        const size_t n = cb_available(&os->cb);

        cb_peek(&os->cb, tmpbuf, n);

        const size_t written = raw_write(os, tmpbuf, n);

        cb_pop(&os->cb, written);
        return written;
}

bool ui_eof(struct ui *is)
{
        return cb_available(&is->cb) == 0 && is->buf_pos == is->buf_end;
}

bool uo_eof(struct uo *os)
{
        return os->buf_pos == os->buf_end;
}

size_t uio_copy(struct ui *src, struct uo *dst)
{
        const size_t dstav = cb_available(&dst->cb);

        if (uo_flush(dst) < dstav)
                return 0;

        char tmpbuf[sizeof(src->cb.buf)];

        size_t r = cb_available(&src->cb);
        cb_peek(&src->cb, tmpbuf, r);
        cb_pop(&src->cb, r);

        size_t w;
        size_t wtot = 0;

        for (;;) {
                w = raw_write(dst, tmpbuf, r);
                wtot += w;

                if (w < r)
                        break;

                r = raw_read(src, tmpbuf, sizeof(tmpbuf));

                if (r == 0) {
                        w = 0;
                        break;
                }
        }

        assert(w < r || (w == 0 && r == 0));

        const size_t n = (size_t) (r - w);

        cb_push(&src->cb, tmpbuf + w, n);

        return wtot;
}

size_t uio_copy_n(struct ui *src, struct uo *dst, size_t n)
{
        for (size_t i = 0; i < n; ++i) {
                char tmp;

                if (!ui_c_peek(src, &tmp) || !uo_c(dst, tmp))
                        return i;

                ui_c(src, NULL);
        }

        uo_flush(dst);
        return n;
}

bool ui_find(struct ui *is, const void *pattern, size_t byte_sz)
{
        const char *const p = pattern;
        size_t i = 0;

        char tmp;

        while (ui_c(is, &tmp)) {
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

#define DEF_PUT_GET_PEEK(TYPE, SUFF)                            \
        bool uo_ ## SUFF(struct uo *os, TYPE v)                 \
        {                                                       \
                return bio_write(os, &v, sizeof(v));            \
        }                                                       \
                                                                \
        bool ui_ ## SUFF(struct ui *is, TYPE *v)                \
        {                                                       \
                if (v && !bio_peek(is, v, sizeof(*v)))          \
                        return false;                           \
                                                                \
                return bio_pop(is, sizeof(*v));                 \
        }                                                       \
                                                                \
        bool ui_ ## SUFF ## _peek(struct ui *is, TYPE *v)       \
        {                                                       \
                return bio_peek(is, v, sizeof(*v));             \
        }

DEF_PUT_GET_PEEK(char, c)
DEF_PUT_GET_PEEK(int, i)
