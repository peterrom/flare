/* Copyright 2014 Peter Román.
   This file is part of Flare which is licensed under GNU GPL v3.
   See the file named LICENSE for details. */

#include "tf.h"
#include "forty.h"
#include "uio.h"

struct testforty_state {
        char buffer[1024];
        char *pos;
};

void testforty_state_init(struct testforty_state *s)
{
        memset(s->buffer, '\0', sizeof(s->buffer));
        s->pos = s->buffer;
}

size_t testforty_state_remaining(struct testforty_state *s)
{
        return (size_t) (s->pos - s->buffer) + sizeof(s->buffer);
}

void testforty_state_write_str(struct testforty_state *s, const char *txt)
{
        const size_t n = strlen(txt);
        memcpy(s->pos, txt, n);
        s->pos += n;
}

void testforty_state_write_uio(struct testforty_state *s, struct ui *txt)
{
        struct uo buf;
        uo_buf(&buf, s->pos, testforty_state_remaining(s));
        s->pos += uio_copy(txt, &buf);
}

void testforty_state_print(void *context, struct ui *arg)
{
        struct testforty_state *s = context;
        testforty_state_write_uio(s, arg);
}

void testforty_state_push(const char *name, void *context)
{
        struct testforty_state *s = context;
        testforty_state_write_str(s, name);
        testforty_state_write_str(s, " ");
}

void testforty_state_pop(const char *name, void *context)
{
        struct testforty_state *s = context;
        testforty_state_write_str(s, " /");
        testforty_state_write_str(s, name);
}

void testforty_state_push_one(void *context)
{
        testforty_state_push("ONE", context);
}

void testforty_state_pop_one(void *context)
{
        testforty_state_pop("ONE", context);
}

void testforty_state_push_two(void *context)
{
        testforty_state_push("TWO", context);
}

void testforty_state_pop_two(void *context)
{
        testforty_state_pop("TWO", context);
}

bool testforty_intrp(const char *input, const char *expected)
{
        struct forty_tag testforty_tl[] = {
                { "one", testforty_state_push_one, testforty_state_pop_one },
                { "two", testforty_state_push_two, testforty_state_pop_two },
                { "onenullpush", NULL, testforty_state_pop_one },
                { "onenullpop", testforty_state_push_one, NULL },
                { NULL, NULL, NULL } /* sentinel */
        };

        struct testforty_state state;
        testforty_state_init(&state);

        struct ui is;
        ui_buf(&is, input, strlen(input));

        forty_parse(testforty_tl, &state, testforty_state_print, &is);

        /* printf("\nexp: \"%s\"\nres: \"%s\"\n", expected, state.buffer); */

        return strncmp(state.buffer, expected, strlen(expected)) == 0;
}

tf_TEST(plain_text)
{
        tf_ASSERT(testforty_intrp("", ""));
        tf_ASSERT(testforty_intrp("some text", "some text"));

        const char long_text[] =
                "And I watched her through the rain coming through "
                "the ceiling and thought back to Salt Lake; and my "
                "father, Sid, who ran the Capitol Theatre. It was a"
                " movie house, but it had been an old vaudeville "
                "house, and he wanted to bring live performances "
                "back to the Capitol. In 1948, he invited Marian "
                "Anderson to come and sing there. And I remembered "
                "we went to the train station to pick her up and "
                "took her to the biggest hotel in town--the Hotel "
                "Utah--but they wouldn’t let her stay there because "
                "she was black. And I remember my father's humiliation, "
                "and her humiliation, as I saw her singing there through "
                "the rain. "
                "And I realized right then; I said \"Brenden right then, "
                "I knew that it was all wrong; that it all had to change. "
                "And that that change had to start with me.\"";

        tf_ASSERT(testforty_intrp(long_text, long_text));
}

tf_TEST(valid_tags)
{
        tf_ASSERT(testforty_intrp("{one }", "ONE  /ONE"));
        tf_ASSERT(testforty_intrp("a {one tag} text", "a ONE tag /ONE text"));

        tf_ASSERT(testforty_intrp("a {one and a {two and a}}",
                                  "a ONE and a TWO and a /TWO /ONE"));

        tf_ASSERT(testforty_intrp("{one {one {two {one }}}}",
                                  "ONE ONE TWO ONE  /ONE /TWO /ONE /ONE"));
}

tf_TEST(null_tags)
{
        tf_ASSERT(testforty_intrp("hmm {onenullpush text} after",
                                  "hmm text /ONE after"));

        tf_ASSERT(testforty_intrp("hmm {onenullpop text} after",
                                  "hmm ONE text after"));
}

tf_TEST(missing_tags)
{
        tf_ASSERT(testforty_intrp("a {missing tag}", "a tag"));

        tf_ASSERT(testforty_intrp("{one {missing {two } two} missing} one",
                                  "ONE TWO  /TWO two missing /ONE one"));

        tf_ASSERT(testforty_intrp("{miss {miss {one {miss }1}2}3}4",
                                  "ONE 1 /ONE234"));
}

tf_TEST(implicit_pops)
{
        tf_ASSERT(testforty_intrp("{one implicit pop",
                                  "ONE implicit pop /ONE"));

        tf_ASSERT(testforty_intrp("{one {missing {two {one ",
                                  "ONE TWO ONE  /ONE /TWO /ONE"));
}

tf_TEST(too_long_tag_names)
{
        tf_ASSERT(testforty_intrp("a {one {really-long-tag-name-will-be-"
                                  "ignored but a short {two not} ignored} one}",
                                  "a ONE but a short TWO "
                                  "not /TWO ignored one /ONE"));
}

tf_TEST(stack_exhaustion)
{
        tf_ASSERT(testforty_intrp("{one 1 {one 2 {one 3 {one 4 {one 5 "
                                  "{one 6 {one 7 {one 8 {one 9 {one 10"
                                  "}10}9}8}7}6}5}4}3}2}1",
                                  "ONE 1 ONE 2 ONE 3 ONE 4 ONE 5 "
                                  "ONE 6 ONE 7 ONE 8 9 10"
                                  "109 /ONE8 /ONE7 /ONE6 /ONE5 "
                                  "/ONE4 /ONE3 /ONE2 /ONE1"));
}

tf_SUITE(forty)
{
        tf_RUN(plain_text);
        tf_RUN(valid_tags);
        tf_RUN(null_tags);
        tf_RUN(missing_tags);
        tf_RUN(implicit_pops);
        tf_RUN(too_long_tag_names);
        tf_RUN(stack_exhaustion);
}
