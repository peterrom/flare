/* Copyright 2016 Peter Román.
   This file is part of Flare which is licensed under GNU GPL v3.
   See the file named LICENSE for details. */

#ifndef SEC_INCL
#define SEC_INCL

/* * Simple Entity-Component (SEC)

   SEC is a basic entity-component world implementation.
 */

#define SEC_MAX_ENT 16

enum sec_component {
        sec_triangle = 1
};

struct sec_world {
        enum sec_component component[SEC_MAX_ENT];

        struct {
                struct {
                        float x, y, z, w;
                } vx[3];
        } triangle[SEC_MAX_ENT];
};

/* New entity index or -1 if no free slot */
int sec_new(struct sec_world *w);

/* Remove entity at index ~i~ */
void sec_del(struct sec_world *w, unsigned int i);

#endif /* SEC_INCL */
