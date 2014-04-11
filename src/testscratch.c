/* Copyright 2014 Peter Román.
   This file is part of Flare which is licensed under GNU GPL v3.
   See the file named LICENSE for details. */

#include "tf.h"
#include "scratch.h"
#include "uio.h"

#include <string.h>

tf_TEST(init)
{
        struct scratch ss;
        scratch_init(&ss);

        tf_ASSERT(ss.valid_beg == ss.buffer);
        tf_ASSERT(ss.valid_end == ss.buffer);
}

tf_TEST(flush_left)
{
        struct scratch ss;
        scratch_init(&ss);

        char data[] = "0123456789";

        struct uio is = uio_mbuf(data, sizeof(data));
        struct uio os = uio_mbuf(ss.buffer, sizeof(ss.buffer));

        ss.valid_end += uio_copy(&is, &os);

        scratch_flush_left(&ss);
        tf_ASSERT(ss.valid_beg == ss.buffer);
        tf_ASSERT(ss.valid_end == ss.buffer + sizeof(data));
        tf_ASSERT(strcmp(ss.buffer, data) == 0);

        ss.valid_beg += 3;

        scratch_flush_left(&ss);

        tf_ASSERT(ss.valid_beg == ss.buffer);
        tf_ASSERT(ss.valid_end == ss.buffer + sizeof(data) - 3);
        tf_ASSERT(strcmp(ss.buffer, "3456789") == 0);
}

tf_TEST(fill_end)
{
        char data[] = "abc";
        struct uio is = uio_mbuf(data, sizeof(data));

        struct scratch ss;
        scratch_init(&ss);

        scratch_fill_end(&ss, &is);

        tf_ASSERT(ss.valid_end == ss.buffer + sizeof(data));
        tf_ASSERT(strcmp(ss.valid_beg, data) == 0);

        is = uio_mbuf(data, sizeof(data));
        ss.valid_end--;

        scratch_fill_end(&ss, &is);
        tf_ASSERT(ss.valid_end == ss.buffer + 2 * sizeof(data) - 1);
        tf_ASSERT(strcmp(ss.valid_beg, "abcabc") == 0);
}

tf_TEST(empty)
{
        struct scratch ss;
        scratch_init(&ss);

        tf_ASSERT(scratch_empty(&ss));

        char data[] = "abc";
        struct uio is = uio_mbuf(data, sizeof(data));
        scratch_fill_end(&ss, &is);

        tf_ASSERT(!scratch_empty(&ss));

        ss.valid_beg += sizeof(data);
        tf_ASSERT(scratch_empty(&ss));
}

tf_TEST(valid_stream)
{
        struct scratch ss;
        scratch_init(&ss);

        struct uio stream = scratch_valid(&ss);
        tf_ASSERT(uio_eof(&stream));

        char data[] = "abc";
        struct uio is = uio_mbuf(data, sizeof(data));
        scratch_fill_end(&ss, &is);

        stream = scratch_valid(&ss);

        char tmp;
        tf_ASSERT(uio_get_c(&stream, &tmp) && tmp == 'a');
        tf_ASSERT(uio_get_c(&stream, &tmp) && tmp == 'b');
        tf_ASSERT(uio_get_c(&stream, &tmp) && tmp == 'c');
        tf_ASSERT(uio_get_c(&stream, &tmp) && tmp == '\0');
        tf_ASSERT(!uio_get_c(&stream, &tmp));

        ss.valid_beg += 3;
        stream = scratch_valid(&ss);

        tf_ASSERT(uio_get_c(&stream, &tmp) && tmp == '\0');
        tf_ASSERT(!uio_get_c(&stream, &tmp));
}

tf_SUITE(scratch)
{
        tf_RUN(init);
        tf_RUN(flush_left);
        tf_RUN(fill_end);
        tf_RUN(empty);
        tf_RUN(valid_stream);
}
