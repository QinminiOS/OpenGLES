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

#ifndef GL_MATH_UTILS_H
#define GL_MATH_UTILS_H

#include "GLMathTypes.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif
    
#define GLM_MAX(x,y) ( ((x)>(y)) ? (x) : (y) )
#define GLM_MIN(x,y) ( ((x)<(y)) ? (x) : (y) )
#define GLM_CLAMP(x, min, max) GLM_MAX(min, GLM_MIN(x, max));

// Utilities
GLM_INLINE GLMFloat degToRad(GLMFloat degrees);
GLM_INLINE GLMFloat radToDeg(GLMFloat radians);
    
GLM_INLINE GLMFloat fastPow(GLMFloat x,GLMFloat y);


mat4_t mat4_perspective(GLMFloat fov_radians, GLMFloat aspect, GLMFloat zNear, GLMFloat zFar);
mat4_t mat4_frustum(GLMFloat left, GLMFloat right, GLMFloat bottom, GLMFloat top, GLMFloat near, GLMFloat far);
// Generates an orthogonal viewing matrix
mat4_t mat4_ortho(GLMFloat left, GLMFloat right, GLMFloat bottom, GLMFloat top, GLMFloat near, GLMFloat far);
// Generates a lookat camera viewing matrix
mat4_t mat4_lookat(GLMFloat eyeX, GLMFloat eyeY, GLMFloat eyeZ,
                   GLMFloat centerX, GLMFloat centerY, GLMFloat centerZ,
                   GLMFloat upX, GLMFloat upY, GLMFloat upZ);

void printVec2(vec2_t vec);
void printVec3(vec3_t vec);
void printVec4(vec4_t vec);
void printMat3(mat3_t mat);
void printMat4(mat4_t mat);
void printQuat(quat_t quat);

GLM_INLINE bool GLMFloatArr_equals(const GLMFloat *a1, const GLMFloat *a2, unsigned int len);
GLM_INLINE bool vec2_equals(const vec2_t v1, const vec2_t v2);
GLM_INLINE bool vec3_equals(const vec3_t v1, const vec3_t v2);
GLM_INLINE bool vec4_equals(const vec4_t v1, const vec4_t v2);
GLM_INLINE bool mat4_equals(const mat4_t m1, const mat4_t m2);
GLM_INLINE bool quat_equals(const quat_t q1, const quat_t q2);

#ifdef GLM_USE_INLINE
#include <GLMath/GLMathUtilitiesInline.c>
#endif
#ifdef __cplusplus
}
#endif
#endif
