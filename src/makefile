# Copyright 2014 Peter Román.
# This file is part of Flare which is licensed under GNU GPL v3.
# See the file named LICENSE for details.

# This (GNU) makefile works by defining the "parts" of a program.
# Each "part" is assumed to consist of (at least) a <name>.c file
# and a test<name>.c file that should define a main function to be
# run by "make check".
#
# Header-only parts should be added to ~H_PARTS~ which will build
# the test for that part but obviously not an object.

# user options
CFLAGS = -Wall -Wextra -pedantic -g -Wshadow -Wwrite-strings   \
	 -Wswitch-enum -Wswitch-default -Wformat=2 -fno-common \
	 -Wconversion

LINK = -lm

CFLAGS += $(shell pkg-config --cflags glfw3)
LINK += $(shell pkg-config --static --libs glfw3)

PARTS += uio
PARTS += forty
PARTS += la
PARTS += chum
PARTS += glenn

H_PARTS += ss

C_PARTS += main

# generated
OBJ = $(addprefix .build/, $(addsuffix .o, ${PARTS}))
EXES = $(addprefix .build/, ${C_PARTS})
TESTS += $(addprefix .build/test, ${PARTS} ${H_PARTS})
DEPS = $(OBJ:.o=.P) $(addsuffix .P, ${TESTS} ${EXES})

${EXES}:

check: ${TESTS}
	@echo
	@echo "  Running test suites"
	@for t in $^; do echo; echo $${t}:; ./$${t}; done

.build/%.P: %.c | .build
	@echo -n .build/ > $@
	@c99 -MM -MP $< >> $@

.build/test%: .build/test%.o ${OBJ}
	@echo "  '-- Linking test: $@"
	@c99 $^ -o $@ ${LINK} ${CFLAGS}
	@echo

.build/%: .build/%.o ${OBJ}
	@echo "  '-- Linking executable: $@"
	@c99 $^ -o $@ ${LINK} ${CFLAGS}
	@echo

.build/%.o: %.c | .build
	@echo Building: $@
	@c99 -c $< -o $@ ${CFLAGS}

.build:
	@mkdir $@

-include ${DEPS}

.PHONY: clean
clean:
	@-rm -rf .build
