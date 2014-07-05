/* Copyright 2014 Peter Román.
   This file is part of Flare which is licensed under GNU GPL v3.
   See the file named LICENSE for details. */

#include "tf.h"
#include "la.h"

tf_TEST(add)
{
        struct la_v2f a = { 1.0, 2.0 };
        struct la_v2f b = { 9.0, 8.0 };
        struct la_v2f c = la_add_v2f(a, b);

        tf_ASSERT(c.v1 == 10.0);
        tf_ASSERT(c.v2 == 10.0);
}

tf_SUITE(la)
{
        tf_RUN(add);
}
