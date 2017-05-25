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

#ifndef GL_MATH_MATRIX_H
#define GL_MATH_MATRIX_H

#ifdef __cplusplus
extern "C" {
#endif

#include "GLMathTypes.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
    
#ifdef GLM_USE_DOUBLE
#define GLM_gesv(...) dgesv_(__VA_ARGS__)
#else
#define GLM_gesv(...) sgesv_(__VA_ARGS__)
#endif

// Matrix math functions
GLM_INLINE mat3_t mat3_mul(const mat3_t m1, const mat3_t m2);
GLM_INLINE vec3_t vec3_mul_mat3(const vec3_t v, const mat3_t m);
GLM_INLINE mat3_t mat3_inverse(const mat3_t m, bool *success_out);
GLM_INLINE mat3_t mat3_transpose(const mat3_t m);
GLM_INLINE GLMFloat mat3_det(const mat3_t m);

GLM_INLINE mat4_t mat4_mul(const mat4_t m1, const mat4_t m2);
GLM_INLINE vec3_t vec3_mul_mat4(const vec3_t v, const mat4_t m, bool isPoint);
GLM_INLINE vec4_t vec4_mul_mat4(const vec4_t v, const mat4_t m);
GLM_INLINE mat4_t mat4_inverse(const mat4_t m, bool *success_out);
GLM_INLINE mat4_t mat4_transpose(const mat4_t m);
GLM_INLINE mat3_t mat4_extract_mat3(const mat4_t m);
GLM_INLINE GLMFloat mat4_det(mat4_t m);

GLM_INLINE mat3_t _mat4_sub_mat3(mat4_t m, int i, int j);

#ifdef GLM_USE_INLINE
#include "GLMathMatrix.c"
#endif

#ifdef __cplusplus
}
#endif
#endif
