/* Copyright 2014 Peter Román.
   This file is part of Flare which is licensed under GNU GPL v3.
   See the file named LICENSE for details. */

#include "tf.h"
#include "uio.h"

tf_TEST(mbuf_put)
{
        int buffer[] = { 0, 0 };
        struct uio os = uio_mbuf(buffer, sizeof(buffer));

        tf_ASSERT(uio_put_i(&os, 1) && buffer[0] == 1);
        tf_ASSERT(uio_put_i(&os, 2) && buffer[1] == 2);
        tf_ASSERT(!uio_put_i(&os, 3));

        tf_ASSERT(buffer[0] == 1 && buffer[1] == 2);
}

tf_TEST(mbuf_get)
{
        int buffer[] = { 1, 2 };

        struct uio is = uio_mbuf(buffer, sizeof(buffer));
        int tmp;

        tf_ASSERT(uio_get_i(&is, &tmp) && tmp == 1);
        tf_ASSERT(uio_get_i(&is, &tmp) && tmp == 2);
        tf_ASSERT(!uio_get_i(&is, &tmp) && tmp == 2);
}

tf_TEST(mbuf_peek)
{
        int buffer[] = { 1, 2 };

        struct uio is = uio_mbuf(buffer, sizeof(buffer));
        int tmp;

        tf_ASSERT(uio_peek_i(&is, &tmp) && tmp == 1);
        tf_ASSERT(uio_peek_i(&is, &tmp) && tmp == 1);
        tf_ASSERT(uio_get_i(&is, NULL));
        tf_ASSERT(uio_peek_i(&is, &tmp) && tmp == 2);
        tf_ASSERT(uio_peek_i(&is, &tmp) && tmp == 2);
        tf_ASSERT(uio_get_i(&is, NULL));
        tf_ASSERT(!uio_peek_i(&is, &tmp));
        tf_ASSERT(!uio_get_i(&is, NULL));
}

tf_TEST(mbuf_eof)
{
        int buffer[] = { 1, 2 };

        struct uio is = uio_mbuf(buffer, sizeof(buffer));
        int tmp;

        tf_ASSERT(!uio_eof(&is));
        tf_ASSERT(uio_get_i(&is, &tmp));
        tf_ASSERT(!uio_eof(&is));
        tf_ASSERT(uio_get_i(&is, &tmp));
        tf_ASSERT(uio_eof(&is));
}

tf_TEST(mbuf_copy)
{
        int buffer1[] = { 1, 2 };
        int buffer2[] = { 3, 4, 5 };

        struct uio os = uio_mbuf(buffer1, sizeof(buffer1));
        struct uio is = uio_mbuf(buffer2, sizeof(buffer2));

        tf_ASSERT(uio_copy_n(&is, &os, sizeof(int)) == sizeof(int) &&
                  buffer1[0] == 3);
}

tf_TEST(mbuf_copy_os_shorter)
{
        int buffer1[] = { 1, 2 };
        int buffer2[] = { 3, 4, 5 };

        struct uio os = uio_mbuf(buffer1, sizeof(buffer1));
        struct uio is = uio_mbuf(buffer2, sizeof(buffer2));

        tf_ASSERT(uio_copy(&is, &os) == sizeof(buffer1) &&
                  buffer1[0] == 3 &&
                  buffer1[1] == 4);

        int tmp;
        tf_ASSERT(uio_get_i(&is, &tmp) && tmp == 5);
        tf_ASSERT(uio_eof(&os));
}

tf_TEST(mbuf_copy_is_shorter)
{
        int buffer1[] = { 1, 2, 3 };
        int buffer2[] = { 4, 5 };

        struct uio os = uio_mbuf(buffer1, sizeof(buffer1));
        struct uio is = uio_mbuf(buffer2, sizeof(buffer2));

        tf_ASSERT(uio_copy(&is, &os) == sizeof(buffer2) &&
                  buffer1[0] == 4 &&
                  buffer1[1] == 5 &&
                  buffer1[2] == 3);

        tf_ASSERT(uio_eof(&is));
        tf_ASSERT(!uio_eof(&os));
}

tf_TEST(find)
{
        char buffer[] = "a long string with a pattern in it";

        struct uio is = uio_mbuf(buffer, sizeof(buffer));

        tf_ASSERT(uio_find(&is, "pattern", 7));
        tf_ASSERT(!uio_eof(&is));
        tf_ASSERT(!uio_find(&is, "pattern", 7));
        tf_ASSERT(uio_eof(&is));
}

tf_SUITE(uio)
{
        tf_RUN(mbuf_put);
        tf_RUN(mbuf_get);
        tf_RUN(mbuf_peek);
        tf_RUN(mbuf_eof);
        tf_RUN(mbuf_copy);
        tf_RUN(mbuf_copy_os_shorter);
        tf_RUN(mbuf_copy_is_shorter);
        tf_RUN(find);
}
