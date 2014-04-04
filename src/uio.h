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

bool uio_put_i(struct uio *s, int v);
bool uio_get_i(struct uio *s, int *v);

#endif /* UIO_INCL */
