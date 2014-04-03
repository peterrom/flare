CFLAGS = -Wall -Wextra

check: .build/testuio
	./$<

.build/testuio: .build/testuio.o .build/uio.o
	c99 $^ -o $@ ${CFLAGS}

.build/%.o: %.c %.h | .build
	c99 -c $< -o $@ ${CFLAGS}

.build/test%.o: test%.c tf.h | .build
	c99 -c $< -o $@ ${CFLAGS}

.build:
	mkdir .build

.PHONY: clean
clean:
	-rm -rf .build
