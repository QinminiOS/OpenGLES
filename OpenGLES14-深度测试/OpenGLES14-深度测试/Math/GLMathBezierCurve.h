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

#ifndef GL_MATH_BEZIER_H
#define GL_MATH_BEZIER_H

#include "GLMath.h"

#ifdef __cplusplus
extern "C" {
#endif

GLM_INLINE bezier_t bezier_create(vec3_t c1, vec3_t c2, vec3_t c3, vec3_t c4);
GLM_INLINE vec3_t bezier_getPoint(bezier_t curve, GLMFloat t);
GLM_INLINE GLMFloat bezier_getCoordForAxis(bezier_t curve, GLMFloat t, bezierAxis_t axis);
GLM_INLINE vec3_t bezier_firstDerivative(bezier_t curve, GLMFloat t);
GLM_INLINE vec2_t bezier_firstDerivativeRoots(bezier_t curve, bezierAxis_t axis);
GLM_INLINE void bezier_extremes(bezier_t curve, vec3_t *outMinimums, vec3_t *outMaximums);
GLM_INLINE vec3_t bezier_getPointWithOffset(bezier_t curve, GLMFloat t, vec3_t offset);
GLM_INLINE void bezier_getLineSegments(bezier_t curve, int count, vec3_t *outPoints, GLMFloat *outLengths, GLMFloat *outDeltas, GLMFloat *outTotalLength);
GLM_INLINE vec3_t bezier_getPointUsingLineSegments(GLMFloat t, int count, vec3_t *points, GLMFloat *lengths, GLMFloat *deltas, GLMFloat totalLength);

#ifdef GLM_USE_INLINE
#include <GLMath/GLMathBezierCurve.c>
#endif

#ifdef __cplusplus
}
#endif
#endif
