// Copyright (c) 2011, Fjölnir Ásgeirsson
//
// Permission to use, copy, modify, and/or distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
// WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
// ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
// WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

#ifndef GL_MATH_VEC_H
#define GL_MATH_VEC_H

#include "GLMathTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

// Vector math functions
GLM_INLINE vec2_t vec2_create(GLMFloat x, GLMFloat y);

GLM_INLINE vec2_t vec2_add(const vec2_t v1, const vec2_t v2);
GLM_INLINE vec2_t vec2_sub(const vec2_t v1, const vec2_t v2);
GLM_INLINE vec2_t vec2_mul(const vec2_t v1, const vec2_t v2);
GLM_INLINE vec2_t vec2_div(const vec2_t v1, const vec2_t v2);
GLM_INLINE GLMFloat vec2_dot(const vec2_t v1, const vec2_t v2);
GLM_INLINE GLMFloat vec2_magSquared(const vec2_t v);
GLM_INLINE GLMFloat vec2_mag(const vec2_t v);
GLM_INLINE vec2_t vec2_normalize(const vec2_t v);
GLM_INLINE GLMFloat vec2_dist(const vec2_t v1, const vec2_t v2);
GLM_INLINE vec2_t vec2_scalarMul(const vec2_t v, GLMFloat s);
GLM_INLINE vec2_t vec2_scalarDiv(const vec2_t v, GLMFloat s);
GLM_INLINE vec2_t vec2_scalarAdd(const vec2_t v, GLMFloat s);
GLM_INLINE vec2_t vec2_scalarSub(const vec2_t v, GLMFloat s);
GLM_INLINE vec2_t vec2_negate(const vec2_t v);
GLM_INLINE vec2_t vec2_floor(vec2_t v);

GLM_INLINE vec3_t vec3_create(GLMFloat x, GLMFloat y, GLMFloat z);
GLM_INLINE vec3_t vec3_add(const vec3_t v1, const vec3_t v2);
GLM_INLINE vec3_t vec3_sub(const vec3_t v1, const vec3_t v2);
GLM_INLINE vec3_t vec3_mul(const vec3_t v1, const vec3_t v2);
GLM_INLINE vec3_t vec3_div(const vec3_t v1, const vec3_t v2);
GLM_INLINE GLMFloat vec3_dot(const vec3_t v1, const vec3_t v2);
GLM_INLINE GLMFloat vec3_magSquared(const vec3_t v);
GLM_INLINE GLMFloat vec3_mag(const vec3_t v);
GLM_INLINE vec3_t vec3_normalize(const vec3_t v);
GLM_INLINE GLMFloat vec3_dist(const vec3_t v1, const vec3_t v2);
GLM_INLINE vec3_t vec3_scalarMul(const vec3_t v, GLMFloat s);
GLM_INLINE vec3_t vec3_scalarDiv(const vec3_t v, GLMFloat s);
GLM_INLINE vec3_t vec3_scalarAdd(const vec3_t v, GLMFloat s);
GLM_INLINE vec3_t vec3_scalarSub(const vec3_t v, GLMFloat s);
GLM_INLINE vec3_t vec3_cross(const vec3_t v1, const vec3_t v2);
GLM_INLINE vec3_t vec3_negate(const vec3_t v);
GLM_INLINE vec3_t vec3_floor(vec3_t v);

GLM_INLINE vec4_t vec4_create(GLMFloat x, GLMFloat y, GLMFloat z, GLMFloat w);
GLM_INLINE vec4_t vec4_add(const vec4_t v1, const vec4_t v2);
GLM_INLINE vec4_t vec4_sub(const vec4_t v1, const vec4_t v2);
GLM_INLINE vec4_t vec4_mul(const vec4_t v1, const vec4_t v2);
GLM_INLINE vec4_t vec4_div(const vec4_t v1, const vec4_t v2);
GLM_INLINE GLMFloat vec4_dot(const vec4_t v1, const vec4_t v2);
GLM_INLINE GLMFloat vec4_magSquared(const vec4_t v);
GLM_INLINE GLMFloat vec4_mag(const vec4_t v);
GLM_INLINE vec4_t vec4_normalize(const vec4_t v);
GLM_INLINE GLMFloat vec4_dist(const vec4_t v1, const vec4_t v2);
GLM_INLINE vec4_t vec4_scalarMul(const vec4_t v, GLMFloat s);
GLM_INLINE vec4_t vec4_scalarDiv(const vec4_t v, GLMFloat s);
GLM_INLINE vec4_t vec4_scalarAdd(const vec4_t v, GLMFloat s);
GLM_INLINE vec4_t vec4_scalarSub(const vec4_t v, GLMFloat s);
GLM_INLINE vec4_t vec4_cross(const vec4_t v1, const vec4_t v2);
GLM_INLINE vec4_t vec4_negate(const vec4_t v);
GLM_INLINE vec4_t vec4_floor(vec4_t v);
GLM_INLINE vec4_t vec4_premultiplyAlpha(vec4_t v);

#ifdef GLM_USE_INLINE
#include <GLMath/GLMathVector.c>
#endif

#ifdef __cplusplus
}
#endif
#endif
