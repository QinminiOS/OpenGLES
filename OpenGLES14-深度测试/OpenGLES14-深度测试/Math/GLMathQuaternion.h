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

#ifndef GL_MATH_QUAT_H
#define GL_MATH_QUAT_H

#include "GLMath.h"
#include <assert.h>
#include <math.h>

#define GLM_EPSILON (1.0E-8)

#ifdef __cplusplus
extern "C" {
#endif

// Quaternion math functions
GLM_INLINE quat_t quat_createf(GLMFloat x, GLMFloat y, GLMFloat z, GLMFloat angle);
GLM_INLINE quat_t quat_createv(vec3_t axis, GLMFloat angle);
GLM_INLINE mat4_t quat_to_mat4(const quat_t q);
GLM_INLINE quat_t mat4_to_quat(const mat4_t m);
GLM_INLINE mat4_t quat_to_ortho(const quat_t q);
GLM_INLINE quat_t ortho_to_quat(const mat4_t m);
GLM_INLINE GLMFloat quat_magSquared(const quat_t q);
GLM_INLINE GLMFloat quat_mag(const quat_t q);
GLM_INLINE quat_t quat_computeW(quat_t q);
GLM_INLINE quat_t quat_normalize(quat_t q);
GLM_INLINE quat_t quat_multQuat(const quat_t qA, const quat_t qB);
GLM_INLINE vec4_t quat_rotatePoint(const quat_t q, const vec4_t v);
GLM_INLINE vec3_t quat_rotateVec3(const quat_t q, const vec3_t v);
GLM_INLINE quat_t quat_inverse(const quat_t q);
GLM_INLINE GLMFloat quat_dotProduct(const quat_t qA, const quat_t qB);
GLM_INLINE quat_t quat_slerp(const quat_t qA, const quat_t qB, GLMFloat t);

#ifdef GLM_USE_INLINE
#include <GLMath/GLMathQuaternion.c>
#endif
#ifdef __cplusplus
}
#endif
#endif
