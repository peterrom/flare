/* Copyright 2014 Peter Román.
   This file is part of Flare which is licensed under GNU GPL v3.
   See the file named LICENSE for details. */

#ifndef GLENN_INCL
#define GLENN_INCL

/* * GL ENgiNe (GLENN)
 */

struct glenn_state {
        unsigned int program;
        unsigned int pbo;
};

void glenn_init(struct glenn_state* s, int width, int height);
void glenn_resize(struct glenn_state* s, int width, int height);
void glenn_display(struct glenn_state* s);

#endif /* GLENN_INCL */
