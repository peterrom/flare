/* Copyright 2014 Peter Román.
   This file is part of Flare which is licensed under GNU GPL v3.
   See the file named LICENSE for details. */

#include "tf.h"
#include "uio.h"

tf_TEST(buf_put)
{
        int buffer[] = { 0, 0 };
        struct uo os;
        tf_ASSERT(uo_buf(&os, buffer, sizeof(buffer)));

        tf_ASSERT(uo_i(&os, 1) && uo_flush(&os) == 4 && buffer[0] == 1);
        tf_ASSERT(uo_i(&os, 2) && uo_flush(&os) == 4 && buffer[1] == 2);

        uo_i(&os, 3);
        tf_ASSERT(uo_flush(&os) == 0);

        tf_ASSERT(buffer[0] == 1 && buffer[1] == 2);
}

tf_TEST(buf_get)
{
        int buffer[] = { 1, 2 };

        struct ui is;
        tf_ASSERT(ui_buf(&is, buffer, sizeof(buffer)));

        int tmp;
        tf_ASSERT(ui_i(&is, &tmp) && tmp == 1);
        tf_ASSERT(ui_i(&is, &tmp) && tmp == 2);
        tf_ASSERT(!ui_i(&is, &tmp) && tmp == 2);
}

tf_TEST(buf_peek)
{
        int buffer[] = { 1, 2 };

        struct ui is;
        tf_ASSERT(ui_buf(&is, buffer, sizeof(buffer)));
        int tmp;

        tf_ASSERT(ui_i_peek(&is, &tmp) && tmp == 1);
        tf_ASSERT(ui_i_peek(&is, &tmp) && tmp == 1);
        tf_ASSERT(ui_i(&is, NULL));
        tf_ASSERT(ui_i_peek(&is, &tmp) && tmp == 2);
        tf_ASSERT(ui_i_peek(&is, &tmp) && tmp == 2);
        tf_ASSERT(ui_i(&is, NULL));
        tf_ASSERT(!ui_i_peek(&is, &tmp));
        tf_ASSERT(!ui_i(&is, NULL));
}

tf_TEST(buf_eof)
{
        int buffer[] = { 1, 2 };

        struct ui is;
        tf_ASSERT(ui_buf(&is, buffer, sizeof(buffer)));

        tf_ASSERT(!ui_eof(&is));
        tf_ASSERT(ui_i(&is, NULL));
        tf_ASSERT(!ui_eof(&is));
        tf_ASSERT(ui_i(&is, NULL));
        tf_ASSERT(ui_eof(&is));
}

tf_TEST(buf_copy)
{
        int buffer1[] = { 1, 2 };
        int buffer2[] = { 3, 4, 5 };

        struct uo os;
        tf_ASSERT(uo_buf(&os, buffer1, sizeof(buffer1)));

        struct ui is;
        tf_ASSERT(ui_buf(&is, buffer2, sizeof(buffer2)));

        tf_ASSERT(uio_copy_n(&is, &os, sizeof(int)) == sizeof(int) &&
                  buffer1[0] == 3);
}

tf_TEST(buf_copy_os_shorter)
{
        int buffer1[] = { 1, 2 };
        int buffer2[] = { 3, 4, 5 };

        struct uo os;
        tf_ASSERT(uo_buf(&os, buffer1, sizeof(buffer1)));

        struct ui is;
        tf_ASSERT(ui_buf(&is, buffer2, sizeof(buffer2)));

        tf_ASSERT(uio_copy(&is, &os) == sizeof(buffer1));
        tf_ASSERT(
                buffer1[0] == 3 &&
                buffer1[1] == 4);

        int tmp;
        tf_ASSERT(ui_i(&is, &tmp) && tmp == 5);
        tf_ASSERT(uo_eof(&os));
}

tf_TEST(buf_copy_is_shorter)
{
        int buffer1[] = { 1, 2, 3 };
        int buffer2[] = { 4, 5 };

        struct uo os;
        tf_ASSERT(uo_buf(&os, buffer1, sizeof(buffer1)));

        struct ui is;
        tf_ASSERT(ui_buf(&is, buffer2, sizeof(buffer2)));

        tf_ASSERT(uio_copy(&is, &os) == sizeof(buffer2) &&
                  buffer1[0] == 4 &&
                  buffer1[1] == 5 &&
                  buffer1[2] == 3);

        tf_ASSERT(ui_eof(&is));
        tf_ASSERT(!uo_eof(&os));
}

tf_TEST(find)
{
        char buffer[] = "a long string with a pattern in it";

        struct ui is;
        tf_ASSERT(ui_buf(&is, buffer, sizeof(buffer)));

        tf_ASSERT(ui_find(&is, "pattern", 7));
        tf_ASSERT(!ui_eof(&is));
        tf_ASSERT(!ui_find(&is, "pattern", 7));
        tf_ASSERT(ui_eof(&is));
}

tf_TEST(cb_rw_wraparound)
{
        tf_ASSERT(!"implemented!");
}

tf_SUITE(uio)
{
        tf_RUN(buf_put);
        tf_RUN(buf_get);
        tf_RUN(buf_peek);
        tf_RUN(buf_eof);
        tf_RUN(buf_copy);
        tf_RUN(buf_copy_os_shorter);
        tf_RUN(buf_copy_is_shorter);
        tf_RUN(find);
        tf_RUN(cb_rw_wraparound);
}
