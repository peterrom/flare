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

tf_TEST(find_any_of)
{
        char data[] = "0123456789";
        struct uio is = uio_mbuf(data, sizeof(data));

        struct scratch ss;
        scratch_init(&ss);

        scratch_fill_end(&ss, &is);

        tf_ASSERT(scratch_find_any_of(&ss, "abc3") == ss.buffer + 3);
        tf_ASSERT(scratch_find_any_of(&ss, "abc") == NULL);

        tf_ASSERT(scratch_find_any_of(&ss, "9") == ss.buffer + 9);
        ss.valid_end -= 2;
        tf_ASSERT(scratch_find_any_of(&ss, "9") == NULL);
}

tf_SUITE(scratch)
{
        tf_RUN(init);
        tf_RUN(flush_left);
        tf_RUN(fill_end);
        tf_RUN(find_any_of);
}
