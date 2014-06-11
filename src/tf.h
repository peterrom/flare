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

   If one of your tests needs some kind of cleanup in the case of a
   triggered ~tf_ASSERT~, that can be accomplished by the
   ~tf_IN_CASE_OF_ASSERT~ macro, i.e.

   : tf_TEST(some_test)
   : {
   :         global_array = malloc(huge_number);
   :
   :         tf_IN_CASE_OF_ASSERT(
   :                 free(global_array);
   :                 );
   :
   :         // perform tests
   :
   :         free(global_array);
   : }

   Note that only the last defined ~tf_IN_CASE_OF_ASSERT~ will be run.

   Since TF sets up a ~main~ function, you are limited to one test
   suite per ~.c~ file.

   TF is designed to be run in an emacs compilation buffer, i.e. M-x
   compile <RET> make check. This is why failure messages are
   formatted as compilation errors. */

#include <stdio.h>
#include <string.h>
#include <setjmp.h>

char tf_g_fail[256] = "";

jmp_buf tf_g_jump_buffer1;
jmp_buf tf_g_jump_buffer2;

#define tf_ASSERT(cond)                                                 \
        do {                                                            \
                if (!(cond)) {                                          \
                        snprintf(tf_g_fail, sizeof(tf_g_fail),          \
                                 "\n" __FILE__ ":%d:1\t\t(" #cond ")"   \
                                 " == false", __LINE__);                \
                                                                        \
                        longjmp(tf_g_jump_buffer2, 1);                  \
                }                                                       \
        } while (0);

#define tf_IN_CASE_OF_ASSERT(body)                      \
        do {                                            \
                if (setjmp(tf_g_jump_buffer2)) {        \
                        body;                           \
                                                        \
                        longjmp(tf_g_jump_buffer1, 1);  \
                }                                       \
        } while (0);                                    \

#define tf_TEST(name)                           \
        void name(void)

#define tf_SUITE(name)                          \
        void tf_test_list(void)

void tf_report_result()
{
        if (strlen(tf_g_fail) > 0) {
                printf(" failed! <<<<<<<<\n%s\n\n", tf_g_fail);
                tf_g_fail[0] = '\0';
        } else {
                printf(" passed!\n");
        }
}

#define tf_RUN(name)                                    \
        do {                                            \
                printf("%-42s", #name);                 \
                fflush(stdout);                         \
                                                        \
                if (!setjmp(tf_g_jump_buffer1))         \
                        if (!setjmp(tf_g_jump_buffer2)) \
                                name();                 \
                                                        \
                tf_report_result();                     \
        } while (0);

void tf_test_list(void);

int main(void)
{
        tf_test_list();
        return 0;
}
