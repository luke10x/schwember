#pragma once

#include "gl.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

typedef struct {
  GLuint ID;
  GLuint unit;
} texture_t;

#include "shader.h"

texture_t* texture_create(
  unsigned char* image_data,
  GLenum unit,
  int width,
  int height,
  int bpp
) {

  // TODO, do we need this at all? If we only work with 2D textures
  GLenum tex_type = GL_TEXTURE_2D;

  texture_t* self = (texture_t*)malloc(sizeof(texture_t));

  // TODO delegate bellow to a private func, 

  glGenTextures(1, &(self->ID));

  // CHECK IF NEEDED
  glActiveTexture(unit);

  // Check again how this is done
  self->unit = unit;

  glBindTexture(GL_TEXTURE_2D, self->ID);

  // TODO support other BPP values
  switch(bpp) {
    case 1:
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
            break;
    case 3:
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
            break;
    case 4:
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
      break;
    default:
      fprintf(stderr, "Unsupported texture BPP value");
      exit(1);
  }

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glGenerateMipmap(GL_TEXTURE_2D);
    
  // Unbind to make suree something else does not interfere
  glBindTexture(GL_TEXTURE_2D, 0);

  stbi_image_free(image_data);

  return self;
}

texture_t* texture_load_from_file(
  const char* file_name,
  GLenum slot,
  GLenum format,
  GLenum pixel_type
) {
  // First load data from file
  stbi_set_flip_vertically_on_load(1);

  int width, height, bpp;
  unsigned char* image_data = stbi_load(file_name, &width, &height, &bpp, 0);

  if (!image_data) {
      printf("Can't load texture from '%s' - %s\n", file_name, stbi_failure_reason());
      exit(1);
  }

  // Then create the texture using loaded data
  return texture_create(image_data, slot, width, height, bpp);
}

void texture_delete(texture_t* self) {
  glDeleteTextures(1, &(self->ID));
}
