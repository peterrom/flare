/* Copyright 2014 Peter Román.
   This file is part of Flare which is licensed under GNU GPL v3.
   See the file named LICENSE for details. */

#ifndef GLENN_INCL
#define GLENN_INCL

/* * GL ENgiNe (GLENN)
 */

struct glenn_state;

void glenn_init(struct glenn_state* s, const unsigned int n);
void glenn_update(struct glenn_state *s, const float *v);
void glenn_resize(struct glenn_state* s, int width, int height);
void glenn_display(struct glenn_state* s);

/* struct glenn_state should be considered opaque. */
struct glenn_state {
        unsigned int program;
        unsigned int pbo;
        unsigned int n;
};

#endif /* GLENN_INCL */
