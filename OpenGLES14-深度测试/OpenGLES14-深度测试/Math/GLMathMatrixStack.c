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

#include "GLMathMatrixStack.h"
#include "GLMathMatrix.h"
#include "GLMathTransforms.h"
#include <unistd.h>

GLM_INLINE matrix_stack_t *matrix_stack_create(unsigned int initialCapacity) {
    matrix_stack_t *out;
    out = malloc(sizeof(matrix_stack_t));
    out->count = 0;
    out->capacity = initialCapacity;
    out->items = malloc(sizeof(mat4_t)*initialCapacity);

    return out;
}

GLM_INLINE void matrix_stack_destroy(matrix_stack_t *stack) {
    free(stack->items);
    free(stack);
}

// Pushes the passed item on to the top of the stack
GLM_INLINE void matrix_stack_push_item(matrix_stack_t *stack, mat4_t item) {
    if((stack->count + 1) == stack->capacity) {
        // If the stack is full we double it's size
        stack->capacity *= 2;
        stack->items = realloc(stack->items, stack->capacity);
    }
    memcpy(&stack->items[stack->count], &item, sizeof(mat4_t));

    ++stack->count;
}

// Pushes an identical copy of the currently topmost item onto the stack (or the identity matrix if no item currently exists)
GLM_INLINE void matrix_stack_push(matrix_stack_t *stack) {
    matrix_stack_push_item(stack, stack->count >= 1 ? stack->items[stack->count-1] : GLMMat4_identity);
}

// Pops an item off the stack
GLM_INLINE void matrix_stack_pop(matrix_stack_t *stack) {
    if(stack->count <= 1)
        return;
    --stack->count;
    // If the stack is far from full free up some memory
    if(stack->capacity > 32 && stack->count < stack->capacity/2) {
        stack->capacity /= 2;
        stack->items = realloc(stack->items, stack->capacity * sizeof(mat4_t));
    }
}
// Returns the topmost item in the stack
GLM_INLINE mat4_t matrix_stack_get_mat4(matrix_stack_t *stack) {
    assert(stack->count > 0);
    return stack->items[stack->count - 1];
}
// Returns the 3x3 portion of the topmost item in the stack
GLM_INLINE mat3_t matrix_stack_get_mat3(matrix_stack_t *stack) {
    assert(stack->count > 0);
    mat4_t *m = &stack->items[stack->count - 1];
    return (mat3_t){ m->m00, m->m01, m->m02,
                     m->m10, m->m11, m->m12,
                     m->m20, m->m21, m->m22 };
}


#pragma mark - Transformation helpers

GLM_INLINE void matrix_stack_mul_mat4(matrix_stack_t *stack, mat4_t m) {
    assert(stack->count > 0);
    stack->items[stack->count-1] = mat4_mul(stack->items[stack->count-1], m);
}

GLM_INLINE void matrix_stack_translate(matrix_stack_t *stack, GLMFloat x, GLMFloat y, GLMFloat z) {
    assert(stack->count > 0);
    stack->items[stack->count-1] = mat4_translate(stack->items[stack->count-1], x, y, z);
}

GLM_INLINE void matrix_stack_rotate(matrix_stack_t *stack, GLMFloat angle, GLMFloat x, GLMFloat y, GLMFloat z) {
    assert(stack->count > 0);
    stack->items[stack->count-1] = mat4_rotate(stack->items[stack->count-1], angle, x, y, z);
}
GLM_INLINE void matrix_stack_scale(matrix_stack_t *stack, GLMFloat x, GLMFloat y, GLMFloat z) {
    assert(stack->count > 0);
    stack->items[stack->count-1] = mat4_scale(stack->items[stack->count-1], x, y, z);
}
