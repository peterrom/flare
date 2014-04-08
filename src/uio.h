/* Copyright 2014 Peter Román.
   This file is part of Flare which is licensed under GNU GPL v3.
   See the file named LICENSE for details. */

#ifndef UIO_INCL
#define UIO_INCL

/* * Unified Input/Output (UIO)
   The idea behind UIO is to have a unified IO model in Flare.
   Whether it is writing/reading to/from memory, files or sockets,
   we want to be able to use the same streaming interface. */

#include <stdbool.h>
#include <stddef.h>

struct uio;

struct uio *uio_open_mbuf(void *mbuf, size_t byte_sz);
void uio_close(struct uio *s);

bool uio_eof(struct uio *s);

/* Copy all or /n/ bytes from /src/ to /dst/. Return the number of
   bytes copied. */
size_t uio_copy(struct uio *src, struct uio *dst);
size_t uio_copy_n(struct uio *src, struct uio *dst, size_t n);

bool uio_put_i(struct uio *s, int v);
bool uio_get_i(struct uio *s, int *v);
bool uio_peek_i(struct uio *s, int *v);

bool uio_put_c(struct uio *s, char v);
bool uio_get_c(struct uio *s, char *v);
bool uio_peek_c(struct uio *s, char *v);

#endif /* UIO_INCL */
