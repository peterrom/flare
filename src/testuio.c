/* Copyright 2014 Peter Román.
   This file is part of Flare which is licensed under GNU GPL v3.
   See the file named LICENSE for details. */

#include "tf.h"
#include "uio.h"

tf_TEST(mbuf_put)
{
        int buffer[] = { 0, 0 };
        struct uio *os = uio_open_mbuf(buffer, sizeof(buffer));

        tf_ASSERT(uio_put_i(os, 1) && buffer[0] == 1);
        tf_ASSERT(uio_put_i(os, 2) && buffer[1] == 2);
        tf_ASSERT(!uio_put_i(os, 3));

        tf_ASSERT(buffer[0] == 1 && buffer[1] == 2);

        uio_close(os);
}

tf_TEST(mbuf_get)
{
        int buffer[] = { 1, 2 };

        struct uio *is = uio_open_mbuf(buffer, sizeof(buffer));
        int tmp;

        tf_ASSERT(uio_get_i(is, &tmp) && tmp == 1);
        tf_ASSERT(uio_get_i(is, &tmp) && tmp == 2);
        tf_ASSERT(!uio_get_i(is, &tmp) && tmp == 2);

        uio_close(is);
}

tf_TEST(mbuf_eof)
{
        int buffer[] = { 1, 2 };

        struct uio *is = uio_open_mbuf(buffer, sizeof(buffer));
        int tmp;

        tf_ASSERT(!uio_eof(is));
        tf_ASSERT(uio_get_i(is, &tmp));
        tf_ASSERT(!uio_eof(is));
        tf_ASSERT(uio_get_i(is, &tmp));
        tf_ASSERT(uio_eof(is));

        uio_close(is);
}

tf_SUITE(uio)
{
        tf_RUN(mbuf_put);
        tf_RUN(mbuf_get);
        tf_RUN(mbuf_eof);
}
