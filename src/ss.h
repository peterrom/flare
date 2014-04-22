/* Copyright 2014 Peter Román.
   This file is part of Flare which is licensed under GNU GPL v3.
   See the file named LICENSE for details. */

#ifndef SS_INCL
#define SS_INCL

/* * Stack Stack (SS)
   SS is a type generic, static, stack allocated, stack implementation.
   To declare a stack, simply use

   : SS_DECLARE_STACK(i, int, 16)

   which will declare the following struct and static interface

   : struct ss_i { ... };
   : static void ss_i_init(struct ss_i *)        { ... }
   : static bool ss_i_full(struct ss_i *)        { ... }
   : static bool ss_i_empty(struct ss_i *)       { ... }
   :
   : // Push ~v~ onto the stack. Return ~true~ if successful.
   : static bool ss_i_push(struct ss_i *, int v) { ... }
   :
   : // Return pointer to top element or ~NULL~.
   : static int *ss_i_top(struct ss_i *)         { ... }
   :
   : // Pop stack. Return pointer to the popped element or ~NULL~.
   : static int *ss_i_pop(struct ss_i *)         { ... }
*/

#include <stdbool.h>

#include "att.h"

#define SS_DECLARE_STACK(NAME, TYPE, SZ)                                \
                                                                        \
        struct ss_ ## NAME {                                            \
                TYPE array[SZ];                                         \
                TYPE *sp;                                               \
        };                                                              \
                                                                        \
        static void ss_ ## NAME ## _init(struct ss_ ## NAME *s)         \
        {                                                               \
                s->sp = s->array;                                       \
        }                                                               \
                                                                        \
        static bool ss_ ## NAME ## _full(struct ss_ ## NAME *s)         \
        {                                                               \
                return s->sp == s->array + SZ;                          \
        }                                                               \
                                                                        \
        static bool ss_ ## NAME ## _empty(struct ss_ ## NAME *s)        \
        {                                                               \
                return s->sp == s->array;                               \
        }                                                               \
                                                                        \
        static bool ss_ ## NAME ## _push(struct ss_ ## NAME *s,         \
                                         TYPE v)                        \
        {                                                               \
                if (ss_ ## NAME ## _full(s))                            \
                        return false;                                   \
                                                                        \
                *s->sp = v;                                             \
                ++s->sp;                                                \
                                                                        \
                return true;                                            \
        }                                                               \
                                                                        \
        att_UNUSED                                                       \
        static TYPE *ss_ ## NAME ## _top(struct ss_ ## NAME *s)         \
        {                                                               \
                if (ss_ ## NAME ## _empty(s))                           \
                        return NULL;                                    \
                                                                        \
                return s->sp - 1;                                       \
        }                                                               \
                                                                        \
        static TYPE *ss_ ## NAME ## _pop(struct ss_ ## NAME *s)         \
        {                                                               \
                if (ss_ ## NAME ## _empty(s))                           \
                        return NULL;                                    \
                                                                        \
                --s->sp;                                                \
                                                                        \
                return s->sp;                                           \
        }

#endif /* SS_INCL */
