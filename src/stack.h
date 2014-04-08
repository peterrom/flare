/* Copyright 2014 Peter Román.
   This file is part of Flare which is licensed under GNU GPL v3.
   See the file named LICENSE for details. */

#ifndef STACK_INCL
#define STACK_INCL

#include <stdbool.h>

#define STACK_DECLARE(TYPE, SZ)                                         \
        struct stack_ ## TYPE {                                         \
                TYPE array[SZ];                                         \
                TYPE *sp;                                               \
        };                                                              \
                                                                        \
        static void stack_ ## TYPE ## _init(struct stack_ ## TYPE *s)   \
        {                                                               \
                s->sp = s->array;                                       \
        }                                                               \
                                                                        \
        static bool stack_ ## TYPE ## _full(struct stack_ ## TYPE *s)   \
        {                                                               \
                return s->sp == s->array + SZ;                          \
        }                                                               \
                                                                        \
        static bool stack_ ## TYPE ## _empty(struct stack_ ## TYPE *s)  \
        {                                                               \
                return s->sp == s->array;                               \
        }                                                               \
                                                                        \
        static bool stack_ ## TYPE ## _push(struct stack_ ## TYPE *s,   \
                                            TYPE v)                     \
        {                                                               \
                if (stack_ ## TYPE ## _full(s))                         \
                        return false;                                   \
                                                                        \
                *s->sp = v;                                             \
                ++s->sp;                                                \
                                                                        \
                return true;                                            \
        }                                                               \
                                                                        \
        static TYPE *stack_ ## TYPE ## _pop(struct stack_ ## TYPE *s)   \
        {                                                               \
                if (stack_ ## TYPE ## _empty(s))                        \
                        return NULL;                                    \
                                                                        \
                --s->sp;                                                \
                                                                        \
                return s->sp;                                           \
        }

#endif /* STACK_INCL */
