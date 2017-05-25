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

#include "GLMathBezierCurve.h"
#include <assert.h>
#include <math.h>

GLM_INLINE bezier_t bezier_create(vec3_t c1, vec3_t c2, vec3_t c3, vec3_t c4)
{
    bezier_t out = { .controlPoints={c1, c2, c3, c4} };
    return out;
}

GLM_INLINE vec3_t bezier_getPoint(bezier_t curve, GLMFloat t)
{
    vec3_t out;
    // Evaluate the bezier curve equation
    GLMFloat mt = 1.0 - t;
    GLMFloat coef0 = powf(mt, 3.0);
    GLMFloat coef1 = 3.0 * powf(mt, 2.0) * t;
    GLMFloat coef2 = 3.0 * mt * powf(t, 2.0);
    GLMFloat coef3 = powf(t, 3.0);

    vec3_t p0 = vec3_scalarMul(curve.controlPoints[0], coef0);
    vec3_t p1 = vec3_scalarMul(curve.controlPoints[1], coef1);
    vec3_t p2 = vec3_scalarMul(curve.controlPoints[2], coef2);
    vec3_t p3 = vec3_scalarMul(curve.controlPoints[3], coef3);

    out = vec3_add(p0, vec3_add(p1, vec3_add(p2, p3)));

    return out;
}

// Calculates only a single component(for the given axis) of a point on the curve
GLM_INLINE GLMFloat bezier_getCoordForAxis(bezier_t curve, GLMFloat t, bezierAxis_t axis)
{
    // Evaluate the bezier curve equation
    GLMFloat mt = 1.0 - t;
    GLMFloat p0 = GLM_FCAST(curve.controlPoints[0])[axis] * powf(mt, 3.0);
    GLMFloat p1 = GLM_FCAST(curve.controlPoints[1])[axis] * (3.0 * powf(mt, 2.0) * t);
    GLMFloat p2 = GLM_FCAST(curve.controlPoints[2])[axis] * (3.0 * mt * powf(t, 2.0));
    GLMFloat p3 = GLM_FCAST(curve.controlPoints[3])[axis] * powf(t, 3.0);
    return p0 + p1 + p2 + p3;
}


GLM_INLINE vec3_t bezier_firstDerivative(bezier_t curve, GLMFloat t)
{
    vec3_t out;
    // Create the derivative curve
    vec3_t derivControlPoints[3];
    derivControlPoints[0] = vec3_sub(curve.controlPoints[1], curve.controlPoints[0]);
    derivControlPoints[1] = vec3_sub(curve.controlPoints[2], curve.controlPoints[1]);
    derivControlPoints[2] = vec3_sub(curve.controlPoints[3], curve.controlPoints[2]);
    // Evaluate the point on the derivative
    GLMFloat mt = 1.0 - t;
    GLMFloat coef0 = powf(mt, 2.0);
    GLMFloat coef1 = 2.0 * mt * t;
    GLMFloat coef2 = powf(t, 2.0);
    vec3_t p0 = vec3_scalarMul(derivControlPoints[0], coef0);
    vec3_t p1 = vec3_scalarMul(derivControlPoints[1], coef1);
    vec3_t p2 = vec3_scalarMul(derivControlPoints[2], coef2);
    out = vec3_add(p0, vec3_add(p1, p2));
    return out;
}
GLM_INLINE vec2_t bezier_firstDerivativeRoots(bezier_t curve, bezierAxis_t axis)
{
    vec2_t out = { -1.0 , -1.0 };
    GLMFloat a = GLM_FCAST(curve.controlPoints[0])[axis];
    GLMFloat b = GLM_FCAST(curve.controlPoints[1])[axis];
    GLMFloat c = GLM_FCAST(curve.controlPoints[2])[axis];
    GLMFloat d = GLM_FCAST(curve.controlPoints[3])[axis];
    GLMFloat tl = -a + 2.0*b - c;
    GLMFloat tr = -sqrtf(-a*(c-d) + b*b - b*(c+d) + c*c);
    GLMFloat denom = -a + 3.0*b - 3.0*c + d;

    if(denom != 0.0) {
        out.x = (tl+tr) / denom;
        out.y = (tl-tr) / denom;
    }
    return out;
}

// Returns the extremes for a curve (minX, minY, minZ) & (maxX, maxY, maxZ)
GLM_INLINE void bezier_extremes(bezier_t curve, vec3_t *outMinimums, vec3_t *outMaximums)
{
    vec3_t start = bezier_getPoint(curve, 0.0);
    vec3_t end   = bezier_getPoint(curve, 1.0);
    vec3_t min, max;
    min.x = GLM_MIN(start.x, end.x);
    min.y = GLM_MIN(start.y, end.y);
    min.z = GLM_MIN(start.z, end.z);
    max.x = GLM_MAX(start.x, end.x);
    max.y = GLM_MAX(start.y, end.y);
    max.z = GLM_MAX(start.z, end.z);
    vec3_t temp;
    for(int axis = 0; axis < 3; ++axis) {
        vec2_t roots = bezier_firstDerivativeRoots(curve, axis);
        for(int i = 0; i < 2; ++i) {
            if(GLM_FCAST(roots)[i] > 0.0 && GLM_FCAST(roots)[i] < 1.0) {
                temp = bezier_getPoint(curve, GLM_FCAST(roots)[i]);
                GLM_FCAST(min)[axis] = GLM_MIN(GLM_FCAST(min)[axis], GLM_FCAST(temp)[axis]);
                GLM_FCAST(max)[axis] = GLM_MAX(GLM_FCAST(max)[axis], GLM_FCAST(temp)[axis]);
            }
        }
    }
    if(outMinimums) *outMinimums = min;
    if(outMaximums) *outMaximums = max;
}
GLM_INLINE vec3_t bezier_getPointWithOffset(bezier_t curve, GLMFloat t, vec3_t offset)
{
    vec3_t tangent = bezier_firstDerivative(curve, t);
    vec3_t normal = { -tangent.y, tangent.x, tangent.z };
    return vec3_add(bezier_getPoint(curve, t), vec3_mul(vec3_normalize(normal), offset));
}

// Computes 'count' many points along with their deltas & the distances between them with an even t interval
GLM_INLINE void bezier_getLineSegments(bezier_t curve, int count,
                                       vec3_t *outPoints, GLMFloat *outLengths, GLMFloat *outDeltas, GLMFloat *outTotalLength)
{    GLMFloat t = 0.0;
    GLMFloat interval = 1.0/(GLMFloat)(count-1);
    for(int i = 0; i < count; ++i) {
        outPoints[i] = bezier_getPoint(curve, t);
        outDeltas[i] = t;
        if(i > 0) {
            outLengths[i-1] = vec3_dist(outPoints[i], outPoints[i-1]);
            *outTotalLength += outLengths[i-1];
        }
        t += interval;
    }
    outLengths[count-1] = 0.0;
}

// Computes a point corresponding to the delta 't' using a list of line segments by interpolating the points
// in the list. This is useful for evaluating a curve at a constant rate when animating (Usually an even delta interval
// on a curve does not produce an even distance)
GLM_INLINE vec3_t bezier_getPointUsingLineSegments(GLMFloat t, int count, vec3_t *points,
                                                   GLMFloat *lengths, GLMFloat *deltas, GLMFloat totalLength)
{
    GLMFloat desiredDistance = t * totalLength;
    assert(desiredDistance >= 0.0 && desiredDistance <= totalLength);
    GLMFloat distanceTravelled = 0.0;
    for(int i = 0; i < count - 1; ++i) {
        GLMFloat nextDistanceTravelled = distanceTravelled + lengths[i];
        if(desiredDistance < nextDistanceTravelled) {
            GLMFloat interpolation = (desiredDistance - distanceTravelled) / lengths[i];
            vec3_t p1 = points[i];
            vec3_t p2 = points[i+1];
            vec3_t movementVector = vec3_scalarMul(vec3_sub(p2, p1), interpolation);
            return vec3_add(p1, movementVector);
        }
        distanceTravelled = nextDistanceTravelled;
    }
    return points[count-1];
}
