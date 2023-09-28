#pragma once

#include "gl.h"

typedef struct {
  float x;
  float y;
  float z;
} vec3_t;

typedef struct {
  float x;
  float y;
} vec2_t;

typedef struct {
	vec3_t position;
	vec3_t color;
	vec2_t texUV;
	float  tex_id;
	vec3_t normal;
} vertex_t;
