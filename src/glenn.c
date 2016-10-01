/* Copyright 2014 Peter Román.
   This file is part of Flare which is licensed under GNU GPL v3.
   See the file named LICENSE for details. */

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>

#define GREG_USE_GLFW3
#include "greg.h"
#include <GLFW/glfw3.h>

#include "glenn.h"
#include "att.h"

static GLuint create_shader(GLenum shader_type, const char *data)
{
        GLuint shader = glCreateShader(shader_type);
        glShaderSource(shader, 1, &data, NULL);

        glCompileShader(shader);

        GLint status;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE) {
		GLchar info_log[256];
		glGetShaderInfoLog(shader, sizeof(info_log), NULL, info_log);

		fprintf(stderr, "Shader compilation failed:\n%s\n", info_log);
		assert(false);
	}
        return shader;
}

static GLuint create_program(int count, ...)
{
        GLuint program = glCreateProgram();

        va_list args1;
        va_start(args1, count);
        va_list args2;
        va_copy(args2, args1);

        for (int i = 0; i < count; ++i)
                glAttachShader(program, va_arg(args1, GLuint));

        glLinkProgram(program);

        GLint status;
        glGetProgramiv(program, GL_LINK_STATUS, &status);
        assert(status != GL_FALSE);

        for (int i = 0; i < count; ++i)
                glDetachShader(program, va_arg(args2, GLuint));

        return program;
}

void glenn_init(struct glenn_state* s, const unsigned int n)
{
        GLuint vertex_shader = create_shader(GL_VERTEX_SHADER,
                                             "#version 330\n"
                                             ""
                                             "layout(location = 0) in vec4 pos;"
                                             ""
                                             "void main()"
                                             "{"
                                             "        gl_Position = pos;"
                                             "}");

        GLuint fragment_shader = create_shader(GL_FRAGMENT_SHADER,
                                               "#version 330\n"
                                               ""
                                               "out vec4 res;"
                                               ""
                                               "void main()"
                                               "{"
                                               "        res = vec4(0.2f,"
                                               "                   0.8f,"
                                               "                   0.2f,"
                                               "                   1.0f);"
                                               "}");

        s->program = create_program(2, vertex_shader, fragment_shader);

        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);

        glGenBuffers(1, &s->pbo);

        s->n = n;

        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
}

void glenn_update(struct glenn_state *s, const float *v)
{
        const size_t sz = s->n * sizeof(float) * 4;

        glBindBuffer(GL_ARRAY_BUFFER, s->pbo);

        /* Buffer re-specification or 'orphaning' */
        glBufferData(GL_ARRAY_BUFFER, sz, NULL, GL_STREAM_DRAW);
        /* Upload new data */
        glBufferData(GL_ARRAY_BUFFER, sz, v, GL_STREAM_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void glenn_resize(att_UNUSED struct glenn_state* s, int width, int height)
{
        glViewport(0, 0, width, height);
}

void glenn_display(struct glenn_state* s)
{
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(s->program);

        glBindBuffer(GL_ARRAY_BUFFER, s->pbo);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

        glDrawArrays(GL_TRIANGLES, 0, s->n);

        glDisableVertexAttribArray(0);
        glUseProgram(0);
}
