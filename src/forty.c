/* Copyright 2014 Peter Román.
   This file is part of Flare which is licensed under GNU GPL v3.
   See the file named LICENSE for details. */

#include "forty.h"

#include <string.h>
#include <assert.h>

#include "uio.h"
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

static void skip_to_next_word(struct ui *is)
{
        char tmp;

        while (ui_peek_c(is, &tmp) && tmp != ' ')
                ui_get_c(is, NULL);

        while (ui_peek_c(is, &tmp) && tmp == ' ')
                ui_get_c(is, NULL);
}

static size_t peek_non_brace(struct ui *is)
{
        char tmp;
        return (ui_peek_c(is, &tmp) && tmp != '{' && tmp != '}') ? 1 : 0;
}

static void handle_text(struct ui *is,
                        void *context, void print(void *, struct ui *))
{
        char tmpbuf[256];

        struct uo tmpbuf_os;
        uo_buf(&tmpbuf_os, tmpbuf, sizeof(tmpbuf));

        struct ui tmpbuf_is;
        ui_buf(&tmpbuf_is, tmpbuf,
               uio_copy_while(is, &tmpbuf_os, peek_non_brace));

        if (print)
                print(context, &tmpbuf_is);
}

static void handle_pops(struct ui *is, void *context,
                        struct ss_ts *ts, int *alt_ts)
{
        char tmp;

        while (ui_peek_c(is, &tmp) && tmp == '}') {
                pop_tag(ts, alt_ts, context);
                ui_get_c(is, NULL);
        }
}

static size_t peek_non_space(struct ui *is)
{
        char tmp;
        return (ui_peek_c(is, &tmp) && tmp != ' ') ? 1 : 0;
}

static void handle_tag(struct ui *is, void *context,
                       const struct forty_tag *tl,
                       struct ss_ts *ts, int *alt_ts)
{
        ui_get_c(is, NULL);

        char tmpbuf[256];

        struct uo tmpbuf_os;
        uo_buf(&tmpbuf_os, tmpbuf, sizeof(tmpbuf));

        const struct forty_tag *tag =
                tl_find(tl, tmpbuf,
                        uio_copy_while(is, &tmpbuf_os, peek_non_space));

        push_tag(ts, alt_ts, tag, context);

        skip_to_next_word(is);
}

static void parse_stream(struct ui *is, struct ss_ts *ts, int *alt_ts,
                         const struct forty_tag *tl,
                         void *context,
                         void print(void *, struct ui *))
{
        char tmp;

        while (ui_peek_c(is, &tmp)) {
                switch (tmp) {
                case '{':
                        handle_tag(is, context, tl, ts, alt_ts);
                        break;
                case '}':
                        handle_pops(is, context, ts, alt_ts);
                        break;
                default:
                        handle_text(is, context, print);
                        break;
                }
        }
}

void forty_parse(const struct forty_tag *tl,
                 void *context,
                 void print(void *, struct ui *),
                 struct ui *is)
{
        struct ss_ts ts;
        ss_ts_init(&ts);

        int alt_ts = 0;

        parse_stream(is, &ts, &alt_ts, tl, context, print);

        pop_remaining(&ts, context);
}
