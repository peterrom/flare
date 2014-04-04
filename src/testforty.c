/* Copyright 2014 Peter Román.
   This file is part of Flare which is licensed under GNU GPL v3.
   See the file named LICENSE for details. */

#include "tf.h"
#include "forty.h"

char testforty_g_buffer[1024];

tf_TEST(no_input)
{
        struct forty_action as[] = {
                { NULL, NULL, NULL}
        };

        struct uio *s = uio_open_mbuf("", sizeof(""));
        tf_ASSERT(forty_print(as, s));
}

tf_SUITE(forty)
{
        tf_RUN(no_input);
}
