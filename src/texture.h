#pragma once

#include "gl.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

typedef struct {
  GLuint ID;
  GLenum type;
  GLuint unit;
} texture_t;

#include "shader.h"

texture_t* texture_create(
  const char* image,
  GLenum tex_type, // texture target 
  GLenum slot,
  GLenum format,
  GLenum pixel_type
);

void texture_unit(texture_t* self, shader_t* shader, const char* uniform, GLuint unit);
void texture_bind(texture_t* self);
void texture_unbind(texture_t* self);
void texture_delete(texture_t* self);

// Implementation //

texture_t* texture_create(
  const char* file_name,
  GLenum tex_type, // texture target 
  GLenum slot,
  GLenum format,
  GLenum pixel_type
) {
  stbi_set_flip_vertically_on_load(1);

  int width, height, bpp;
  unsigned char* image_data = stbi_load(file_name, &width, &height, &bpp, 0);

  if (!image_data) {
      printf("Can't load texture from '%s' - %s\n", file_name, stbi_failure_reason());
      exit(1);
  }

  // printf("Width %d, height %d, bpp %d\n", width, height, bpp);

  texture_t* self = (texture_t*)malloc(sizeof(texture_t));
  self->type = tex_type;

  // TODO delegate bellow to a private func, 

  glGenTextures(1, &(self->ID));

  // CHECK IF NEEDED
  glActiveTexture(slot);

  // Check again how this is done
  self->unit = slot;

  glBindTexture(self->type, self->ID);

  // TODO support other BPP values
  switch(bpp) {
    case 1:
      glTexImage2D(self->type, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
            break;
    case 3:
      glTexImage2D(self->type, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
            break;
    case 4:
      glTexImage2D(self->type, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
      break;
    default:
      fprintf(stderr, "Unsupported texture BPP value");
      exit(1);
  }


  glTexParameteri(self->type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(self->type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameterf(self->type, GL_TEXTURE_BASE_LEVEL, 0);
  glTexParameteri(self->type, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(self->type, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glGenerateMipmap(self->type);
    
  // Unbind to make suree something else does not interfere
  glBindTexture(self->type, 0);

  stbi_image_free(image_data);

  return self;
}


// should not be in this "class"
void texture_unit(texture_t* self, shader_t* shader, const char* uniform, GLuint unit) {
  GLuint location = glGetUniformLocation(shader->ID, uniform);
  // shader_activate(shader);
  glUniform1i(location, unit);
}

void texture_bind(texture_t* self) {
  // TODO improve on what we call slot and texture
  glActiveTexture(self->unit);
  glBindTexture(self->type, self->ID);
}

void texture_unbind(texture_t* self) {
  glBindTexture(self->type, 0);
}

void texture_delete(texture_t* self) {
  glDeleteTextures(1, &(self->ID));
}
