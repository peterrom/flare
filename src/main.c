/* Copyright 2014 Peter Román.
   This file is part of Flare which is licensed under GNU GPL v3.
   See the file named LICENSE for details. */

#include <stdio.h>
#include <stdlib.h>

#include <GLFW/glfw3.h>

#include "att.h"

static void error_callback(int error, const char* description)
{
        fprintf(stderr, "error %d\tdescription %s\n", error, description);
}

static void key_callback(GLFWwindow* window, int key, att_UNUSED int scancode,
                         int action, att_UNUSED int mods)
{
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
                glfwSetWindowShouldClose(window, GL_TRUE);
}

int main(void)
{
        glfwSetErrorCallback(error_callback);

        if (!glfwInit())
                exit(EXIT_FAILURE);

        GLFWwindow *window = glfwCreateWindow(640, 480, "Simple example",
                                              NULL, NULL);

        if (!window) {
                glfwTerminate();
                exit(EXIT_FAILURE);
        }

        glfwMakeContextCurrent(window);
        glfwSetKeyCallback(window, key_callback);

        while (!glfwWindowShouldClose(window)) {
                int width;
                int height;
                glfwGetFramebufferSize(window, &width, &height);

                float ratio = width / (float) height;
                glViewport(0, 0, width, height);
                glClear(GL_COLOR_BUFFER_BIT);
                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
                glRotatef((float) glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
                glBegin(GL_TRIANGLES);
                glColor3f(1.f, 0.f, 0.f);
                glVertex3f(-0.6f, -0.4f, 0.f);
                glColor3f(0.f, 1.f, 0.f);
                glVertex3f(0.6f, -0.4f, 0.f);
                glColor3f(0.f, 0.f, 1.f);
                glVertex3f(0.f, 0.6f, 0.f);
                glEnd();

                glfwSwapBuffers(window);
                glfwPollEvents();
        }

        glfwDestroyWindow(window);
        glfwTerminate();
        return 0;
}
