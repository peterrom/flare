/* Copyright 2014 Peter Román.
   This file is part of Flare which is licensed under GNU GPL v3.
   See the file named LICENSE for details. */

#include "tf.h"
#include "uio.h"

int testuio_asc_mem[] = { 0,  1,  2,  3,  4,  5,  6,  7,
                          8,  9,  10, 11, 12, 13, 14, 15,
                          16, 17, 18, 19, 20, 21, 22, 23,
                          24, 25, 26, 27, 28, 29, 30, 31,
                          32, 33, 34, 35, 36, 37, 38, 39 };

int testuio_buffer[1024];

void testuio_init_ibuf(struct ui *is, size_t n)
{
        tf_ASSERT(n * sizeof(int) <= sizeof(testuio_asc_mem));
        tf_ASSERT(ui_buf(is, testuio_asc_mem, n * sizeof(int)));
}

void testuio_init_obuf(struct uo *os, size_t n)
{
        memset(testuio_buffer, 0xff, sizeof(testuio_buffer));

        tf_ASSERT(n <= sizeof(testuio_buffer));
        tf_ASSERT(uo_buf(os, testuio_buffer, n * sizeof(int)));
}

bool testuio_is_ascending_buf(int first, int last)
{
        struct ui is;
        ui_buf(&is, testuio_buffer, sizeof(testuio_buffer));

        int tmp;

        for (int i = first; i <= last; ++i) {
                const bool res = ui_get_i(&is, &tmp);

                /* printf("\ni == %d: res == %d, tmp == %d", i, res, tmp); */

                if (!res || tmp != i)
                        return false;
        }

        return true;
}

void testuio_get_put(struct ui *is, struct uo *os)
{
        size_t n = 0;

        int tmp;
        while (ui_get_i(is, &tmp) && uo_put_i(os, tmp))
                n++;

        uo_flush(os);
}

void testuio_copy(struct ui *is, struct uo *os)
{
        uio_copy(is, os);
}

struct testuio_subtest {
        void (*f) (struct ui *, struct uo *);

        const size_t is_size;
        const size_t os_size;

        const int res_first;
        const int res_last;
};

void testuio_subsuite(void init_is(struct ui *, size_t),
                      void init_os(struct uo *, size_t),
                      bool is_ascending(int, int))
{
        struct ui is;
        struct uo os;

        const struct testuio_subtest subtests[] = {
                { testuio_get_put, 40, 40, 0, 39 },
                { testuio_get_put, 20, 40, 0, 19 },
                { testuio_get_put, 40, 20, 0, 19 },
                { testuio_copy, 40, 40, 0, 39 },
                { testuio_copy, 20, 40, 0, 19 },
                { testuio_copy, 40, 20, 0, 19 },
                { NULL, 0, 0, 0, 0 } /* sentinel */
        };

        for (const struct testuio_subtest *st = subtests; st->f; ++st) {
                init_is(&is, st->is_size);

                tf_IN_CASE_OF_ASSERT(
                        ui_close(&is);
                        );

                init_os(&os, st->os_size);

                tf_IN_CASE_OF_ASSERT(
                        ui_close(&is);
                        uo_close(&os);
                        );

                st->f(&is, &os);
                tf_ASSERT(is_ascending(st->res_first, st->res_last));

                ui_close(&is);
                uo_close(&os);
        }
}

tf_TEST(buf_buf)
{
        testuio_subsuite(testuio_init_ibuf, testuio_init_obuf,
                         testuio_is_ascending_buf);
}

tf_SUITE(uio)
{
        tf_RUN(buf_buf);
}
