# Copyright 2014 Peter Román.
# This file is part of Flare which is licensed under GNU GPL v3.
# See the file named LICENSE for details.

# This (GNU) makefile works by defining the "parts" of a program.
# Each "part" is assumed to consist of (at least) a <name>.c file
# and a test<name>.c file that should define a main function to be
# run by "make check".

# user options
CFLAGS = -Wall -Wextra

PARTS += uio
PARTS += forty

# generated
OBJ = $(addprefix .build/, $(addsuffix .o, ${PARTS}))
TESTS = $(addprefix .build/test, ${PARTS})
DEPS = $(OBJ:.o=.P) $(addsuffix .P, ${TESTS})

check: ${TESTS}
	@for t in $^; do echo; echo $${t}; ./$${t}; done

.build/%.P: %.c | .build
	@echo -n .build/ > $@
	c99 -MM -MP $< >> $@

.build/test%: .build/test%.o ${OBJ}
	c99 $^ -o $@ ${CFLAGS}

.build/%.o: %.c | .build
	c99 -c $< -o $@ ${CFLAGS}

.build/test%.o: test%.c | .build
	c99 -c $< -o $@ ${CFLAGS}

.build:
	mkdir .build

-include ${DEPS}

.PHONY: clean
clean:
	-rm -rf .build
