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

tf_TEST(fill)
{
        char data[] = "abc";
        struct uio is = uio_mbuf(data, sizeof(data));

        struct scratch ss;
        scratch_init(&ss);

        scratch_fill(&ss, &is);

        tf_ASSERT(ss.valid_end == ss.buffer + sizeof(data));
        tf_ASSERT(strcmp(ss.valid_beg, data) == 0);

        is = uio_mbuf(data, sizeof(data));
        ss.valid_beg++;
        ss.valid_end--;

        scratch_fill(&ss, &is);
        tf_ASSERT(ss.valid_end == ss.buffer + 2 * sizeof(data) - 1 - 1);
        tf_ASSERT(strcmp(ss.valid_beg, "bcabc") == 0);
        tf_ASSERT(strcmp(ss.buffer, "bcabc") == 0);
}

tf_TEST(empty)
{
        struct scratch ss;
        scratch_init(&ss);

        tf_ASSERT(scratch_empty(&ss));

        char data[] = "abc";
        struct uio is = uio_mbuf(data, sizeof(data));
        scratch_fill(&ss, &is);

        tf_ASSERT(!scratch_empty(&ss));

        ss.valid_beg += sizeof(data);
        tf_ASSERT(scratch_empty(&ss));
}

tf_TEST(valid)
{
        struct scratch ss;
        scratch_init(&ss);

        struct uio stream = scratch_valid(&ss);
        tf_ASSERT(uio_eof(&stream));

        char data[] = "abc";
        struct uio is = uio_mbuf(data, sizeof(data));
        scratch_fill(&ss, &is);

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

tf_TEST(full)
{
        struct scratch ss;
        scratch_init(&ss);

        char data[sizeof(ss.buffer)];
        memset(data, 'c', sizeof(data));

        struct uio is = uio_mbuf(data, sizeof(data));
        tf_ASSERT(scratch_fill(&ss, &is) == sizeof(ss.buffer));
        tf_ASSERT(scratch_full(&ss));

        ss.valid_beg++;

        tf_ASSERT(!scratch_full(&ss));
}

tf_TEST(clear)
{
        struct scratch ss;
        scratch_init(&ss);

        char data[] = "abc";
        struct uio is = uio_mbuf(data, sizeof(data));
        scratch_fill(&ss, &is);

        tf_ASSERT(!scratch_empty(&ss));
        scratch_clear(&ss);
        tf_ASSERT(scratch_empty(&ss));
}

tf_SUITE(scratch)
{
        tf_RUN(init);
        tf_RUN(fill);
        tf_RUN(empty);
        tf_RUN(valid);
        tf_RUN(full);
        tf_RUN(clear);
}
