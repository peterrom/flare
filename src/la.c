/* Copyright 2014 Peter Román.
   This file is part of Flare which is licensed under GNU GPL v3.
   See the file named LICENSE for details. */

#include <math.h>

#include "la.h"

struct la_v2f la_add_v2f(struct la_v2f a, struct la_v2f b)
{
        return (struct la_v2f) { a.v1 + b.v1, a.v2 + b.v2 };
}

struct la_v2f la_sub_v2f(struct la_v2f a, struct la_v2f b)
{
        return (struct la_v2f) { a.v1 - b.v1, a.v2 - b.v2 };
}

struct la_v2f la_mul_2f(struct la_m2f m, struct la_v2f v)
{
        return (struct la_v2f) { m.m11 * v.v1 + m.m12 * v.v2,
                        m.m21 * v.v1 + m.m22 * v.v2 };
}

float la_dot_v2f(struct la_v2f a, struct la_v2f b)
{
        return a.v1 * b.v1 + a.v2 * b.v2;
}

float la_norm2_v2f(struct la_v2f a)
{
        return la_dot_v2f(a, a);
}

float la_norm_v2f(struct la_v2f a)
{
        return sqrtf(la_norm2_v2f(a));
}
