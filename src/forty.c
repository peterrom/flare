#include "forty.h"

#include <string.h>


struct action_stack {
        struct forty_action array[8];
        struct forty_action *sp;
};

static bool as_full(struct action_stack *s)
{
        return s->sp == s->array + sizeof(s->array);
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
        ++s->sp;

        return true;
}

static struct forty_action *as_pop(struct action_stack *s)
{
        if (as_empty(s))
                return NULL;

        --s->sp;

        return s->sp;
}

static struct forty_action *find_action(struct forty_action *actions,
					const char *id)
{
        for (; actions->id; ++actions)
                if (strcmp(actions->id, id) == 0)
                        return actions;

        return NULL;
}

bool forty_print(const struct forty_action *actions, struct uio *s)
{
        return true;
}
