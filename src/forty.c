/* Copyright 2014 Peter Román.
   This file is part of Flare which is licensed under GNU GPL v3.
   See the file named LICENSE for details. */

#include "forty.h"

#include <string.h>
#include <assert.h>

#include "uio.h"
#include "scratch.h"
#include "ss.h"

/* ~ts~ is a tag stack. In addition to functioning as a "normal"
   stack, it can keep track of "invalid" pushes/pops, i.e. pushes of
   missing or ill-formated tags where you want the pop order to still
   be correct. */

struct ts_element {
        const struct forty_tag *tp;
        unsigned int invalid_pushes;
};

SS_DECLARE_STACK(ts, struct ts_element, 8)

static void push_invalid(struct ss_ts *ts)
{
        if (ss_ts_empty(ts)) {
                struct ts_element dummy = { NULL, 0 };
                ss_ts_push(ts, dummy);
        }

        ++ss_ts_top(ts)->invalid_pushes;
}

static bool is_dummy(struct ts_element *e)
{
        return !e->tp;
}

static bool pop_invalid(struct ss_ts *ts)
{
        assert(!ss_ts_empty(ts));

        struct ts_element *top = ss_ts_top(ts);

        if (top->invalid_pushes == 0)
                return false;

        --top->invalid_pushes;

        if (top->invalid_pushes == 0 && is_dummy(top))
                ss_ts_pop(ts);

        return true;
}

static void push_tag(struct ss_ts *ts,
                     const struct forty_tag *tag,
                     void *context)
{
        struct ts_element e = {
                .tp = tag,
                .invalid_pushes = 0
        };

        if (tag && ss_ts_push(ts, e)) {
                if (tag->push)
                        tag->push(context);
        } else {
                push_invalid(ts);
        }
}

static void pop_tag(struct ss_ts *ts, void *context)
{
        if (ss_ts_empty(ts) || pop_invalid(ts))
                return;

        const struct forty_tag *tag = ss_ts_pop(ts)->tp;

        if (tag && tag->pop)
                tag->pop(context);
}

static void pop_remaining(struct ss_ts *ts, void *context)
{
        struct ts_element *e;

        while ((e = ss_ts_pop(ts)))
                if (e->tp && e->tp->pop)
                        e->tp->pop(context);
}

static const struct forty_tag *tl_find(const struct forty_tag *tl,
                                       const char *id, size_t sz)
{
        for (; tl->id; ++tl)
                if (strncmp(tl->id, id, sz) == 0)
                        return tl;

        return NULL;
}

static bool not_brace(char c)
{
        return c != '{' && c != '}';
}

static char *first_brace_or_end(char *beg, char *end)
{
        while (beg != end && not_brace(*beg))
                ++beg;

        return beg;
}

static char *handle_text(struct scratch *s,
                         void *context, void print(void *, struct ui *))
{
        char *end = first_brace_or_end(s->valid_beg, s->valid_end);

        if (print) {
                assert(end >= s->valid_beg);

                struct ui arg;
                ui_buf(&arg, s->valid_beg, (size_t) (end - s->valid_beg));
                print(context, &arg);
        }

        return end;
}

static char *handle_pops(struct scratch *s, void *context, struct ss_ts *ts)
{
        char *end = s->valid_beg;
        for (; end != s->valid_end && *end == '}'; ++end)
                pop_tag(ts, context);

        return end;
}

static char *tag_end_or_end(char *beg, char *end)
{
        while (beg != end && *beg != ' ')
                ++beg;

        return beg;
}

static char *handle_tag(struct scratch *s, void *context,
                        const struct forty_tag *tl,
                        struct ss_ts *ts)
{
        char *const beg = s->valid_beg + 1;
        char *const tag_end = tag_end_or_end(beg, s->valid_end);

        if (tag_end == s->valid_end)
                return NULL;

        const struct forty_tag *tag =
                tl_find(tl, beg, (size_t) (tag_end - beg));

        push_tag(ts, tag, context);

        return tag_end + 1;
}

static bool parse_scratch(struct scratch *s, struct ss_ts *ts,
                          const struct forty_tag *tl,
                          void *context,
                          void print(void *, struct ui *))
{
        while (!scratch_empty(s)) {
                char *end;

                switch (*s->valid_beg) {
                case '{':
                        end = handle_tag(s, context, tl, ts);
                        break;
                case '}':
                        end = handle_pops(s, context, ts);
                        break;
                default:
                        end = handle_text(s, context, print);
                        break;
                }

                if (!end)
                        return false;

                s->valid_beg = end;
        }

        return true;
}

static void reset_state(struct ui *is, struct scratch *s, struct ss_ts *ts)
{
        ui_find(is, " ", 1);
        scratch_clear(s);
        push_invalid(ts);
}

void forty_parse(const struct forty_tag *tl,
                 void *context,
                 void print(void *, struct ui *),
                 struct ui *is)
{
        struct scratch s;
        scratch_init(&s);

        struct ss_ts ts;
        ss_ts_init(&ts);

        while (scratch_fill(&s, is)) {
                if (!parse_scratch(&s, &ts, tl, context, print) &&
                    scratch_full(&s))
                        reset_state(is, &s, &ts);
        }

        pop_remaining(&ts, context);
}
