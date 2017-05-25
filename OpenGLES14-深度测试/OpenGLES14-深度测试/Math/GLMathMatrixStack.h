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

#ifndef GL_MATH_STACK_H
#define GL_MATH_STACK_H

#include "GLMathTypes.h"
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

GLM_INLINE matrix_stack_t *matrix_stack_create(unsigned int initialCapacity);
GLM_INLINE void matrix_stack_destroy(matrix_stack_t *stack);

// Pushes the passed matrix on to the top of the stack
GLM_INLINE void matrix_stack_push_item(matrix_stack_t *stack, mat4_t item);
// Pushes an identical copy of the currently topmost matrix onto the stack (or the identity matrix if no item currently exists)
GLM_INLINE void matrix_stack_push(matrix_stack_t *stack);
// Pops an item off the stack
GLM_INLINE void matrix_stack_pop(matrix_stack_t *stack);
// Returns the topmost item in the stack
GLM_INLINE mat4_t matrix_stack_get_mat4(matrix_stack_t *stack);
// Returns the 3x3 portion of the topmost item in the stack
GLM_INLINE mat3_t matrix_stack_get_mat3(matrix_stack_t *stack);

GLM_INLINE void matrix_stack_mul_mat4(matrix_stack_t *stack, mat4_t m);
GLM_INLINE void matrix_stack_translate(matrix_stack_t *stack, GLMFloat x, GLMFloat y, GLMFloat z);
GLM_INLINE void matrix_stack_rotate(matrix_stack_t *stack, GLMFloat angle, GLMFloat x, GLMFloat y, GLMFloat z);
GLM_INLINE void matrix_stack_scale(matrix_stack_t *stack, GLMFloat x, GLMFloat y, GLMFloat z);

#ifdef GLM_USE_INLINE
#include "GLMathMatrixStack.c>
#endif

#ifdef __cplusplus
}
#endif
#endif
