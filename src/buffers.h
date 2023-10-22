#pragma once

#include "gl.h"

typedef struct {
    float x;
    float y;
    float z;
} vec3_t;

typedef float vec4_t[4];
typedef GLuint uvec4_t[4];

typedef struct {
    float x;
    float y;
} vec2_t;

typedef struct {
    vec3_t position;
    vec3_t color;
    vec2_t texUV;
    float tex_id;
    vec3_t normal;
    uvec4_t weights;
    vec4_t bones;
} vertex_t;
