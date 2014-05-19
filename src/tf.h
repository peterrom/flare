/* Copyright 2014 Peter Román.
   This file is part of Flare which is licensed under GNU GPL v3.
   See the file named LICENSE for details. */

#ifdef TF_INCL
#error "You cannot include tf.h twice!"
#endif

#define TF_INCL

/* * Test Framework (TF)
   This is the minimalistic test framework used in Flare. It works by
   defining tests as

   : tf_TEST(true_is_true)
   : {
   :         tf_ASSERT(true);
   : }

   and then setting up a suite

   : tf_SUITE(example)
   : {
   :         tf_RUN(true_is_true);
   : }

   and that's it!

   Since tf_SUITE sets up a ~main~ function, TF is limited to one suite
   per ~.c~ file.

   The suites are designed to be run in an emacs compilation buffer. That
   is why failure messages are formatted as compilation errors. */

#include <stdio.h>
#include <string.h>
#include <setjmp.h>

char tf_g_fail[256] = "";
jmp_buf tf_g_jump_buffer;

#define tf_ASSERT(cond)                                                 \
        do {                                                            \
                if (!(cond)) {                                          \
                        snprintf(tf_g_fail, 256,                        \
                                 "\n" __FILE__ ":%d:1\t\t(" #cond ")"   \
                                 " == false", __LINE__);                \
                                                                        \
                        longjmp(tf_g_jump_buffer, 1);                   \
                }                                                       \
        } while (0);

#define tf_TEST(name)                           \
        void name(void)

#define tf_SUITE(name)                          \
        void tf_test_list(void)

void tf_report_result(const char *name)
{
        if (strlen(tf_g_fail) > 0) {
                printf("%-42s failed! <<<<<<<<\n%s\n\n", name, tf_g_fail);
                tf_g_fail[0] = '\0';
        } else {
                printf("%-42s passed!\n", name);
        }
}

#define tf_RUN(name)                            \
        do {                                    \
                if (!setjmp(tf_g_jump_buffer))  \
                        name();                 \
                                                \
                tf_report_result(#name);        \
        } while (0);

void tf_test_list(void);

int main(void)
{
        tf_test_list();
        return 0;
}
