/* Copyright 2014 Peter Román.
   This file is part of Flare which is licensed under GNU GPL v3.
   See the file named LICENSE for details. */

#include "tf.h"
#include "ss.h"

SS_DECLARE_STACK(i, int, 3)

tf_TEST(init_empty)
{
        struct ss_i s;
        ss_i_init(&s);

        tf_ASSERT(ss_i_empty(&s));
}

tf_TEST(push_pop)
{
        struct ss_i s;
        ss_i_init(&s);

        tf_ASSERT(ss_i_push(&s, 1));
        tf_ASSERT(ss_i_push(&s, 2));
        tf_ASSERT(ss_i_push(&s, 3));
        tf_ASSERT(!ss_i_push(&s, 4));

        tf_ASSERT(*ss_i_pop(&s) == 3);
        tf_ASSERT(*ss_i_pop(&s) == 2);
        tf_ASSERT(*ss_i_pop(&s) == 1);
        tf_ASSERT(!ss_i_pop(&s));
}

struct testss_struct {
        int v;
};

SS_DECLARE_STACK(tst, struct testss_struct, 2)

tf_TEST(ss_of_struct)
{
        struct ss_tst s;
        ss_tst_init(&s);
        tf_ASSERT(ss_tst_empty(&s));

        struct testss_struct a = { 1 };
        struct testss_struct b = { 2 };

        tf_ASSERT(ss_tst_push(&s, a));
        tf_ASSERT(ss_tst_push(&s, b));
        tf_ASSERT(!ss_tst_push(&s, b));

        tf_ASSERT(ss_tst_pop(&s)->v == 2);
        tf_ASSERT(ss_tst_pop(&s)->v == 1);
        tf_ASSERT(!ss_tst_pop(&s));
}

SS_DECLARE_STACK(pi, int *, 2)

tf_TEST(ss_of_pointers)
{
        struct ss_pi s;
        ss_pi_init(&s);
        tf_ASSERT(ss_pi_empty(&s));

        int a = 1;
        int b = 2;

        tf_ASSERT(ss_pi_push(&s, &a));
        tf_ASSERT(ss_pi_push(&s, &b));
        tf_ASSERT(!ss_pi_push(&s, &b));

        tf_ASSERT(**ss_pi_pop(&s) == 2);
        tf_ASSERT(**ss_pi_pop(&s) == 1);
        tf_ASSERT(!ss_pi_pop(&s));
}

tf_SUITE(ss)
{
        tf_RUN(init_empty);
        tf_RUN(push_pop);
        tf_RUN(ss_of_struct);
        tf_RUN(ss_of_pointers);
}
