/* Copyright 2014 Peter Román.
   This file is part of Flare which is licensed under GNU GPL v3.
   See the file named LICENSE for details. */

#include "tf.h"
#include "stack.h"

STACK_DECLARE(int, 3)

tf_TEST(init_empty)
{
        struct stack_int s;
        stack_int_init(&s);

        tf_ASSERT(stack_int_empty(&s));
}

tf_TEST(push_pop)
{
        struct stack_int s;
        stack_int_init(&s);

        tf_ASSERT(stack_int_push(&s, 1));
        tf_ASSERT(stack_int_push(&s, 2));
        tf_ASSERT(stack_int_push(&s, 3));
        tf_ASSERT(!stack_int_push(&s, 4));

        tf_ASSERT(*stack_int_pop(&s) == 3);
        tf_ASSERT(*stack_int_pop(&s) == 2);
        tf_ASSERT(*stack_int_pop(&s) == 1);
        tf_ASSERT(!stack_int_pop(&s));
}

tf_SUITE(stack)
{
        tf_RUN(init_empty);
        tf_RUN(push_pop);
}
