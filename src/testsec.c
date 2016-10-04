/* Copyright 2016 Peter Román.
   This file is part of Flare which is licensed under GNU GPL v3.
   See the file named LICENSE for details. */

#include "tf.h"
#include "sec.h"

tf_TEST(new)
{
        struct sec_world w;
        sec_world_init(&w);

        for (int i = 0; i < SEC_MAX_ENT; ++i) {
                int index = sec_new(&w);
                tf_ASSERT(index == i);
                w.component[index] = sec_c_triangle;
        }

        tf_ASSERT(sec_new(&w) == -1);
}

tf_TEST(del)
{
        struct sec_world w;
        sec_world_init(&w);

        int i;
        while ((i = sec_new(&w)) != -1)
                w.component[i] = sec_c_triangle;

        sec_del(&w, 5);
        tf_ASSERT(sec_new(&w) == 5);
}

tf_SUITE(sec)
{
        tf_RUN(new);
        tf_RUN(del);
}
