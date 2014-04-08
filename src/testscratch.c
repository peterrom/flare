/* Copyright 2014 Peter Román.
   This file is part of Flare which is licensed under GNU GPL v3.
   See the file named LICENSE for details. */

#include "tf.h"
#include "scratch.h"
#include "uio.h"

#include <string.h>

tf_TEST(flush_left)
{
        struct scratch ss;
        scratch_init(&ss);

        tf_ASSERT(ss.valid_beg == ss.buffer);
        tf_ASSERT(ss.valid_end == ss.buffer);

        const char data[] = "0123456789";

        struct uio is = uio_mbuf("0123456789", sizeof("0123456789"));
        struct uio os = uio_mbuf(ss.buffer, sizeof(ss.buffer));

        ss.valid_end += uio_copy(&is, &os);
        ss.valid_beg += 3;

        scratch_flush_left(&ss);

        tf_ASSERT(ss.valid_beg == ss.buffer);
        tf_ASSERT(ss.valid_end == ss.buffer + sizeof(data) - 3);
        tf_ASSERT(strcmp(ss.buffer, "3456789") == 0);
}

tf_SUITE(scratch)
{
        tf_RUN(flush_left);
}
