/* Copyright 2014 Peter Román.
   This file is part of Flare which is licensed under GNU GPL v3.
   See the file named LICENSE for details. */

#include "tf.h"
#include "chum.h"

tf_TEST(init)
{
        struct chum_context ctx;
        tf_ASSERT(chum_init(&ctx));

        tf_IN_CASE_OF_ASSERT(
                chum_terminate(&ctx);
                );

        tf_ASSERT(chum_refresh(&ctx));

        chum_terminate(&ctx);
}

tf_SUITE(chum)
{
        tf_RUN(init);
}
