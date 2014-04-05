# Copyright 2014 Peter Román.
# This file is part of Flare which is licensed under GNU GPL v3.
# See the file named LICENSE for details.

# user options
CFLAGS = -Wall -Wextra

PARTS += uio
PARTS += forty

# generated
OBJ = $(addprefix .build/, $(addsuffix .o, ${PARTS}))
DEPS = $(OBJ:.o=.P)
TESTS = $(addprefix .build/test, ${PARTS})

check: ${TESTS}
	@for t in $^; do echo; echo $${t}; ./$${t}; done

.build/%.P: %.c | .build
	@echo -n .build/ > $@
	@c99 -MM -MP $< >> $@

.build/test%: .build/test%.o ${OBJ}
	c99 $^ -o $@ ${CFLAGS}

.build/%.o: %.c %.h | .build
	c99 -c $< -o $@ ${CFLAGS}

.build/test%.o: test%.c tf.h | .build
	c99 -c $< -o $@ ${CFLAGS}

.build:
	mkdir .build

-include ${DEPS}

.PHONY: clean
clean:
	-rm -rf .build
