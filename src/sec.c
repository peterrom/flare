/* Copyright 2016 Peter Román.
   This file is part of Flare which is licensed under GNU GPL v3.
   See the file named LICENSE for details. */

#include "sec.h"

void sec_world_init(struct sec_world *w)
{
        *w = (struct sec_world) { 0 };
}

int sec_new(struct sec_world *w)
{
        for (int i = 0; i < SEC_MAX_ENT; ++i)
                if (!w->component[i])
                        return i;

        return -1;
}

void sec_del(struct sec_world *w, unsigned int i)
{
        w->component[i] = 0;
}
