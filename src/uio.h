/* Copyright 2014 Peter Román.
   This file is part of Flare which is licensed under GNU GPL v3.
   See the file named LICENSE for details. */

#ifndef UIO_INCL
#define UIO_INCL

/* * Unified Input/Output (UIO)

   The idea behind UIO is to have a unified IO model in Flare.
   Whether it is writing/reading to/from memory, files, sockets,
   handling user input or inter-thread communication we want to be
   able to use the same streaming interface. */

#include <stdbool.h>
#include <stddef.h>

struct ui;
struct uo;

/* Stream initialization methods. Return ~true~ if stream was
   successfully initialized. */
bool ui_null(struct ui *is);
bool uo_null(struct uo *os);

bool ui_buf(struct ui *is, const void *buf, size_t byte_sz);
bool uo_buf(struct uo *os, void *buf, size_t byte_sz);

/* bool ui_file(struct ui *is, const char *filename); */
/* bool uo_file(struct uo *os, const char *filename); */

/* bool uio_socket(struct ui *is, struct uo *os, */
/*              const char *ifc, unsigned int port); */

/* bool uio_fifo(struct ui *is, struct uo *os, void *buf, size_t byte_sz); */

void ui_close(struct ui *is);
void uo_close(struct uo *os);

size_t uo_flush(struct uo *os);

bool ui_eof(struct ui *is);
bool uo_eof(struct uo *os);

/* Return a pointer to the first input stream where data is available
   or NULL in case of a timeout. */
/* struct ui *ui_select(long timeout_us, struct ui *is1, ...); */

/* Copy all or ~n~ bytes from ~src~ to ~dst~. Return the number of
   bytes copied. */
size_t uio_copy(struct ui *src, struct uo *dst);
size_t uio_copy_n(struct ui *src, struct uo *dst, size_t n);

/* Copy data from ~src~ to ~dst~ while ~peek~ returns non-zero
   chunk sizes. */
size_t uio_copy_while(struct ui *src, struct uo *dst,
                      size_t peek(struct ui *));

/* Find the first occurrence of ~pattern~ in ~is~. Return ~true~ if
   found.  The stream will be positioned directly after the pattern or
   at eof. */
bool ui_find(struct ui *is, const void *pattern, size_t byte_sz);

bool uo_put_c(struct uo *os, char v);
bool ui_get_c(struct ui *is, char *v);
bool ui_peek_c(struct ui *is, char *v);

bool uo_put_i(struct uo *os, int v);
bool ui_get_i(struct ui *is, int *v);
bool ui_peek_i(struct ui *is, int *v);

/* The layout of ~struct ui~ and ~struct uo~ are presented here since
   we want to be able to place them on the stack. They should however
   be treated as opaque objects; accessing or modifying them directly
   results undefined behaviour when using the UIO API. */

struct uio_cb {
        size_t w;
        size_t r;

        char buf[64];
};

struct ui {
        /* int fd; */
        const char *buf_pos;
        const char *buf_end;

        struct uio_cb cb;
};

struct uo {
        /* int fd; */
        char *buf_pos;
        char *buf_end;

        struct uio_cb cb;
};

#endif /* UIO_INCL */
