/* Copyright 2014 Peter Román.
   This file is part of Flare which is licensed under GNU GPL v3.
   See the file named LICENSE for details. */

#include "forty.h"

#include <string.h>
#include <assert.h>

#include "uio.h"
#include "scratch.h"
#include "ss.h"

SS_DECLARE_STACK(ts, const struct forty_tag *, 8)

static void push_tag(struct ss_ts *ts,
                     int *alt_ts,
                     const struct forty_tag *tag,
                     void *context)
{
        if (ss_ts_push(ts, tag)) {
                if (tag && tag->push)
                        tag->push(context);
        } else {
                ++(*alt_ts);
        }
}

static void pop_tag(struct ss_ts *ts, int *alt_ts, void *context)
{
        assert(!ss_ts_empty(ts));

        if (*alt_ts > 0) {
                --(*alt_ts);
                return;
        }

        const struct forty_tag *tag = *ss_ts_pop(ts);

        if (tag && tag->pop)
                tag->pop(context);
}

static void pop_remaining(struct ss_ts *ts, void *context)
{
        const struct forty_tag **element;

        while ((element = ss_ts_pop(ts))) {
                const struct forty_tag *tag = *element;

                if (tag && tag->pop)
                        tag->pop(context);

        }
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
        char *const end = first_brace_or_end(s->valid_beg, s->valid_end);

        if (print) {
                struct ui arg;
                ui_buf(&arg, s->valid_beg, (size_t) (end - s->valid_beg));

                print(context, &arg);
        }

        return end;
}

static char *handle_pops(struct scratch *s, void *context,
                         struct ss_ts *ts, int *alt_ts)
{
        char *i = s->valid_beg;

        for (; i != s->valid_end && *i == '}'; ++i)
                pop_tag(ts, alt_ts, context);

        return i;
}

static char *tag_end_or_end(char *beg, char *end)
{
        char *i = beg;
        while (i != end && *i != ' ')
                ++i;

        return i;
}

static char *handle_tag(struct scratch *s, void *context,
                        const struct forty_tag *tl,
                        struct ss_ts *ts, int *alt_ts)
{
        char *const tag_beg = s->valid_beg + 1;
        char *const tag_end = tag_end_or_end(tag_beg, s->valid_end);

        if (tag_end == s->valid_end)
                return NULL;

        const struct forty_tag *tag =
                tl_find(tl, tag_beg, (size_t) (tag_end - tag_beg));

        push_tag(ts, alt_ts, tag, context);

        return tag_end + 1;
}

static bool parse_scratch(struct scratch *s, struct ss_ts *ts, int *alt_ts,
                          const struct forty_tag *tl,
                          void *context,
                          void print(void *, struct ui *))
{
        while (!scratch_empty(s)) {
                char *end;

                switch (*s->valid_beg) {
                case '{':
                        end = handle_tag(s, context, tl, ts, alt_ts);
                        break;
                case '}':
                        end = handle_pops(s, context, ts, alt_ts);
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

static void reset_state(struct ui *is, struct scratch *s,
                        struct ss_ts *ts, int *alt_ts)
{
        ui_find(is, " ", 1);
        scratch_clear(s);
        push_tag(ts, alt_ts, NULL, NULL);
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

        int alt_ts = 0;

        while (scratch_fill(&s, is)) {
                if (!parse_scratch(&s, &ts, &alt_ts, tl, context, print) &&
                    scratch_full(&s))
                        reset_state(is, &s, &ts, &alt_ts);
        }

        pop_remaining(&ts, context);
}
