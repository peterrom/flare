#include "forty.h"

#define ACTION_STACK_SZ 8

struct action_stack {
        struct forty_action array[ACTION_STACK_SZ];
        struct forty_action *sp;
};

static bool as_full(struct action_stack *s)
{
        return s->sp == s->array + ACTION_STACK_SZ;
}

static bool as_empty(struct action_stack *s)
{
        return s->sp == s->array;
}

static bool as_push(struct action_stack *s, struct forty_action *a)
{
        if (as_full(s))
                return false;

        s->sp = a;
        s->sp += 1;

        return true;
}

static struct forty_action *as_pop(struct action_stack *s)
{
        if (as_empty(s))
                return NULL;

        s->sp -= 1;

        return s->sp;
}

bool forty_print(const struct forty_action *actions, struct uio *s)
{
        return true;
}
