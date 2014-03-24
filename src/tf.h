/* Copyright 2014 Peter Román.
   This file is part of Flare which is licensed under GNU GPL v3.
   See the file named LICENSE for details. */

#ifdef TF_INCL
#error "You cannot include tf.h twice!"
#endif

#define TF_INCL

#include <stdio.h>
#include <string.h>

char tf_g_fail[256] = "";

#define tf_ASSERT(cond)                                                 \
        do {                                                            \
                if (!(cond)) {                                          \
                        snprintf(tf_g_fail, 256,                        \
                                 "\n" __FILE__ ":%d:1\t\t(" #cond ")"   \
                                 " == false", __LINE__);                \
                        return;                                         \
                }                                                       \
        } while (0);

#define tf_TEST(name)                           \
        void name(void)

#define tf_SUITE(name)                          \
        void tf_test_list(void)

void tf_report_result(const char *name)
{
        if (strlen(tf_g_fail) > 0) {
                printf("%s failed!\n%s\n", name, tf_g_fail);
                tf_g_fail[0] = '\0';
        } else {
                printf("%s passed!\n", name);
        }
}

#define tf_RUN(name)                            \
        do {                                    \
                name();                         \
                tf_report_result(#name);        \
        } while (0);

void tf_test_list(void);

int main(void)
{
        tf_test_list();
        return 0;
}
