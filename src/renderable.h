#pragma once

#include <stdbool.h>

#include "buffers.h"
#include "camera.h"
#include "gl.h"
#include "texture.h"

typedef struct {
    texture_t* albedo_texture;
    GLuint vao_id;
    GLsizei index_count;
} renderable_t;

renderable_t* renderable_create(
    vertex_t* vertices,
    GLsizei vertex_count,
    GLuint* indices,
    GLsizei index_count,
    texture_t* albedo_texture
)
{
    renderable_t* self = (renderable_t*) malloc(sizeof(renderable_t));

    self->index_count = index_count;

    self->albedo_texture = NULL;
    if (albedo_texture != NULL) {
        self->albedo_texture = (texture_t*) malloc(sizeof(texture_t));
        texture_t* dest      = self->albedo_texture;
        memcpy(dest, albedo_texture, sizeof(texture_t));
    }

    // Create VAO
    glGenVertexArrays(1, &(self->vao_id));
    glBindVertexArray(self->vao_id);

    // Create VBO with vertices
    GLuint vbo_id;
    glGenBuffers(1, &vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    size_t vbo_size = vertex_count * sizeof(vertex_t);
    glBufferData(GL_ARRAY_BUFFER, vbo_size, vertices, GL_STATIC_DRAW);

    // Create EBO with indexes
    GLuint ebo_id;
    glGenBuffers(1, &ebo_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id);
    size_t ebo_size = self->index_count * sizeof(GLuint);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER, ebo_size, indices, GL_STATIC_DRAW
    );

    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);

    // Links VBO attributes such as coordinates and colors to VAO
    // clang-format off

    // What is the second param ??? 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)  0);                  // position
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)( 3 * sizeof(float))); // color
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)( 6 * sizeof(float))); // tex_UV
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)( 8 * sizeof(float))); // tex_id
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)( 9 * sizeof(float))); // normal
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)(12 * sizeof(float))); // 4 weights
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)(16 * sizeof(float))); // 4 bones
    // clang-format on

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    glEnableVertexAttribArray(5);
    glEnableVertexAttribArray(6);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Unbind all to prevent accidentally modifying them
    glBindVertexArray(0);                      // VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);          // VBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);  // EBO

    return self;
}

void renderable_draw(
    renderable_t* self,
    shader_t* shader,
    camera_t* camera
)
{
    shader_activate(shader);
    
    glUniform1i(glGetUniformLocation(shader->ID, "u_albedoTexture"), 0);
    glUniform1i(glGetUniformLocation(shader->ID, "u_ambientOcclusionTexture"), 1);

    // Renderable may or may not have Albedo (base color) texture...
    bool has_albedo_texture = self->albedo_texture != NULL;
    // ...either way set the flag as uniform 
    glUniform1i(
        glGetUniformLocation(shader->ID, "u_hasAlbedoTexture"),
        has_albedo_texture
    );
    // If it has albedo texture, also bind it to this shader
    if (has_albedo_texture) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, self->albedo_texture->ID);
    }

    // Set camera-position uniform value
    glUniform3f(
        glGetUniformLocation(shader->ID, "camPos"),  // uniform location
        camera->view_matrix[3][0],  // camera position X
        camera->view_matrix[3][1],  // camera position Y
        camera->view_matrix[3][2]   // camera position Z
    );

    // Set camera-view-to-world-transfomation matrix unifrom value
    glUniformMatrix4fv(
        glGetUniformLocation(
            shader->ID, "worldToView"
        ),                                     // uniform location
        1,                                     // number of matrices
        GL_FALSE,                              // transpose
        glm::value_ptr(camera->camera_matrix)  // value
    );

    glBindVertexArray(self->vao_id);
    glDrawElements(
        GL_TRIANGLES,       // Mode, OpenGL ES 2
        self->index_count,  // Index count
        GL_UNSIGNED_INT,    // Data type of indices array
        (void*) (0 * sizeof(unsigned int))  // Indices pointer
    );
}
