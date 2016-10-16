# Copyright 2016 Peter Román.
# This file is part of Flare which is licensed under GNU GPL v3.
# See the file named LICENSE for details.

CFLAGS := \
	-Wall \
	-Wextra \
	-pedantic \
	-g \
	-Wshadow \
	-Wwrite-strings \
	-Wswitch-enum \
	-Wswitch-default \
	-Wformat=2 \
	-fno-common

CPPFLAGS := \
	-I../lib/glfw/include

LDFLAGS := \
	-ldl \
	-lX11 \
	-lXrandr \
	-lXinerama \
	-lXcursor \
	-lGL \
	-pthread \
	-lm

GREG_SRC := \
	../lib/greg/deps/pugixml.cpp \
	../lib/greg/deps/wire.cpp \
	../lib/greg/src/greg.cpp

GLFW_OBJ := \
	glfw_context.o \
	glfw_egl_context.o \
	glfw_glx_context.o \
	glfw_init.o \
	glfw_input.o \
	glfw_linux_joystick.o \
	glfw_monitor.o \
	glfw_posix_time.o \
	glfw_posix_tls.o \
	glfw_vulkan.o \
	glfw_window.o \
	glfw_x11_init.o \
	glfw_x11_monitor.o \
	glfw_x11_window.o \
	glfw_xkb_unicode.o

FLARE_SRC := \
	chum.c \
	forty.c \
	glenn.c \
	la.c \
	lime.c \
	sec.c \
	uio.c

FLARE_TEST := \
	testchum \
	testforty \
	testglenn \
	testla \
	testsec \
	testss \
	testuio

.PHONY: all
all: greg.h flare

greg: ../lib/greg
	$(CXX) -std=c++11 -I$</deps $(GREG_SRC) -o $@

greg.h KHR: greg
	cd ../lib/greg && ../../src/greg --api=gl --core --version=3.3
	mv ../lib/greg/output/greg.h .
	ln -s  ../lib/greg/output/KHR

glfw_%.o: ../lib/glfw/src/%.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -D_GLFW_X11 $< -o $@

flare: main.c $(FLARE_SRC) $(GLFW_OBJ)
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ $(LDFLAGS) -o $@

test%: test%.c $(FLARE_SRC) $(GLFW_OBJ)
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ $(LDFLAGS) -o $@

.PHONY: run
run: flare
	./flare

.PHONY: check
check: $(FLARE_TEST)
	for t in $^; do echo; echo $${t}:; ./$${t}; done
