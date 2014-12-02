/* Copyright 2014 Peter Román.
   This file is part of Flare which is licensed under GNU GPL v3.
   See the file named LICENSE for details. */

#include <stdio.h>

#define GREG_IMPLEMENTATION
#define GREG_USE_GLFW3
#include "greg.h"
#include <GLFW/glfw3.h>

#include "chum.h"
#include "att.h"

static void error_callback(int error, const char* description)
{
        fprintf(stderr, "error: %d\tdesc: %s\n", error, description);
}

static void key_callback(GLFWwindow* window, int key, att_UNUSED int scancode,
                         int action, att_UNUSED int mods)
{
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
                glfwSetWindowShouldClose(window, GL_TRUE);
}

bool chum_init(struct chum_context *ctx)
{
        glfwSetErrorCallback(error_callback);

        if (!glfwInit())
                return false;

        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        ctx->internal = glfwCreateWindow(640, 480, "Flare", NULL, NULL);

        if (!ctx->internal) {
                glfwTerminate();
                return false;
        }

        glfwMakeContextCurrent(ctx->internal);
        glfwSetKeyCallback(ctx->internal, key_callback);

	if (!gregInit())
		return false;

        return true;
}

void chum_terminate(struct chum_context *ctx)
{
        glfwDestroyWindow(ctx->internal);
        glfwTerminate();
}

bool chum_refresh(struct chum_context *ctx)
{
        glfwSwapBuffers(ctx->internal);
        glfwPollEvents();

        return !glfwWindowShouldClose(ctx->internal);
}

void chum_window_size(struct chum_context *ctx, int *width, int *height)
{
        glfwGetFramebufferSize(ctx->internal, width, height);
}
