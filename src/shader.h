#pragma once

#include <GLFW/glfw3.h>

typedef struct {
    GLuint ID;
} shader_t;

shader_t*
shader_create(const char* vertex_file, const char* fragment_file);

void shader_activate(const shader_t* self);

void shader_delete(const shader_t* self);

// Implementation

#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gl.h"
#include "shader.h"

char* _get_file_contents(const char* filename)
{
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error opening file %s\n", filename);
        return NULL;
    }

    fseek(fp, 0L, SEEK_END);
    long file_size = ftell(fp);
    rewind(fp);

    char* buffer = (char*) malloc(sizeof(char) * (file_size + 1));
    if (buffer == NULL) {
        perror("Error allocating memory");
        fclose(fp);
        return NULL;
    }

    size_t result = fread(buffer, 1, file_size, fp);
    if (result != file_size) {
        perror("Error reading file");
        fclose(fp);
        free(buffer);
        return NULL;
    }

    buffer[file_size] = '\0';
    fclose(fp);

// There is $VERSION placeholder in the file,
// at position 9.
#ifdef __EMSCRIPTEN__
    char version[9] = "300 es  ";
#else
    char version[9] = "330 core";
#endif
    memcpy(&buffer[9], version, 8);

    return buffer;
}

void _print_compile_errors(unsigned int shader, int isProgram)
{
    // Stores status of compilation
    GLint hasCompiled;
    // Character array to store error message in
    char infoLog[1024];
    if (isProgram > 0) {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
        if (hasCompiled == GL_FALSE) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            fprintf(
                stderr, "SHADER_COMPILATION_ERROR for: %d\n %s\n",
                isProgram, infoLog
            );
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
        if (hasCompiled == GL_FALSE) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            fprintf(
                stderr, "SHADER_LINKING_ERROR for: %d\n %s\n",
                isProgram, infoLog
            );
        }
    }
}

GLuint _loadShader(GLenum type, const char* source)
{
    // create shader
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        printf("gl error %d\n", error);
    }
    GLuint shader = glCreateShader(type);
    if (shader == 0) {
        fprintf(stderr, "Error creating shader\n");
        return 0;
    }

    // load the shader source to the shader object and compile it;
    glShaderSource(shader, 1, &source, NULL);
    //~ glShaderSource(shader, 1, &source, 0);
    glCompileShader(shader);
    _print_compile_errors(shader, type);
    // check if the shader compiled successfully
    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        fprintf(stderr, "Shader compilation error\n");
        glDeleteShader(shader);
        return 0;
    } else { /* shader compiled, BAU */
    }
    return shader;
}

GLuint _buildProgram(
    GLuint vertexShader,
    GLuint fragmentShader,
    const char* vertexPositionName
)
{
    // create a GL program and link it
    GLuint po = glCreateProgram();
    glAttachShader(po, vertexShader);
    glAttachShader(po, fragmentShader);
    glBindAttribLocation(po, 0, vertexPositionName);
    glLinkProgram(po);

    // check if the program linked successfully
    GLint linked;
    glGetProgramiv(po, GL_LINK_STATUS, &linked);
    if (!linked) {
        _print_compile_errors(po, 0);

        glDeleteProgram(po);
        return 0;
    }
    return po;
}

void shader_set_uniform_bone_to_world_transforms(shader_t* self);

shader_t*
shader_create(const char* vertex_file, const char* fragment_file)
{
    char* vertex_source   = _get_file_contents(vertex_file);
    char* fragment_source = _get_file_contents(fragment_file);

    // load vertex and fragment shaders
    // fprintf(stdout, "Compiling vertex shader: %s\n", vertex_file);
    GLuint vertexShader = _loadShader(GL_VERTEX_SHADER, vertex_source);
    // fprintf(stdout, "Compiling fragment shader: %s\n",
    // fragment_file);
    GLuint fragmentShader =
        _loadShader(GL_FRAGMENT_SHADER, fragment_source);

    shader_t* self = (shader_t*) malloc(sizeof(shader_t));
    self->ID =
        _buildProgram(vertexShader, fragmentShader, "v_position");

    shader_set_uniform_bone_to_world_transforms(self);
    return self;
}

void shader_activate(const shader_t* self) { glUseProgram(self->ID); }

void shader_set_uniform_mat4(
    shader_t* self,
    const char* uniform,
    glm::mat4 value
)
{
    shader_activate(self);

    glUniformMatrix4fv(
        glGetUniformLocation(self->ID, uniform),  // location
        1,                                        // count
        GL_FALSE,                                 // transpose
        (GLfloat*) glm::value_ptr(value)          // value
    );
}

void shader_set_uniform_vec4(
    shader_t* self,
    const char* uniform,
    glm::vec4 value
)
{
    shader_activate(self);

    glUniform4f(
        glGetUniformLocation(self->ID, uniform),  // location
        value[0], value[1], value[2], value[3]    // value
    );
}

void shader_set_uniform_vec3(
    shader_t* self,
    const char* uniform,
    glm::vec3 value
)
{
    shader_activate(self);

    glUniform3f(
        glGetUniformLocation(self->ID, uniform),  // location
        value[0], value[1], value[2]              // value
    );
}

/* *********************************************************************
 * sets u_selectedJointIndex uniform param
 * ********************************************************************/
void shader_set_uniform_selected_joint_index(
    shader_t* self,
    GLuint selected_joint_index
)
{
    glUseProgram(self->ID);
    glUniform1ui(
        glGetUniformLocation(self->ID, "u_selectedJointIndex"),  // Loc
        selected_joint_index  // value
    );
}

/* *********************************************************************
 * sets u_boneToWorldTransforms uniform array
 * ********************************************************************/
void shader_set_uniform_bone_to_world_transforms(
    shader_t* self
    // glm::mat4* bone_to_world_transforms_buffer
)
{
    glm::mat4 bone_to_world_transforms_buffer__[32];

    // Fill the array with identity matrices
    for (int i = 0; i < 32; i++) {
        if (i > 2) {
            bone_to_world_transforms_buffer__[i] =
                glm::mat4(4.0f);  // Identity matrix

        } else
            bone_to_world_transforms_buffer__[i] =
                glm::mat4(1.0f);  // Identity matrix
    }

    glUseProgram(self->ID);

    for (int i = 0; i < 32; i++) {
        // uniform name
        char uniformName[128];
        snprintf(
            uniformName,                    // output buffer
            128,                            // max chars to write
            "u_boneToWorldTransforms[%d]",  // format
            i                               // params
        );

        // Sent uniform
        float* uniform_value =
            glm::value_ptr(bone_to_world_transforms_buffer__[i]);
        glUniformMatrix4fv(
            glGetUniformLocation(self->ID, uniformName),  // Location
            1,         // count (TODO, see if we can do bul using this)
            GL_FALSE,  // Transpose?
            uniform_value  // a pointer to 16 float values
        );
    }
}

void shader_delete(const shader_t* self) {}
