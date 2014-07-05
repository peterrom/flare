/* Copyright 2014 Peter Román.
   This file is part of Flare which is licensed under GNU GPL v3.
   See the file named LICENSE for details. */

#ifndef LA_INCL
#define LA_INCL

/* * Linear Algebra (LA)
 */

struct la_v2f {
        float v1;
        float v2;
};

struct la_m2f {
        float m11, m12;
        float m21, m22;
};

struct la_v2f la_add_v2f(struct la_v2f a, struct la_v2f b);
struct la_v2f la_sub_v2f(struct la_v2f a, struct la_v2f b);
struct la_v2f la_mul_2f(struct la_m2f m, struct la_v2f v);
float la_dot_v2f(struct la_v2f a, struct la_v2f b);
float la_norm2_v2f(struct la_v2f a);
float la_norm_v2f(struct la_v2f a);

#endif /* LA_INCL */
