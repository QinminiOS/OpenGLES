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

#include "GLMathTypes.h"
#include "GLMathVector.h"
#include "GLMathMatrix.h"
#include "GLMathQuaternion.h"
#include "GLMathBezierCurve.h"
#include "GLMathMatrixStack.h"
#include "GLMathUtilities.h"
#include <math.h>
#ifdef GLM_USE_ACCELERATE_FRAMEWORK
#import <Accelerate/Accelerate.h>
#endif

#pragma mark - ~ Vectors(2)
GLM_INLINE vec2_t vec2_create(GLMFloat x, GLMFloat y) {
    vec2_t out = { x, y };
    return out;
}

GLM_INLINE vec2_t vec2_add(const vec2_t v1, const vec2_t v2) {
#ifdef GLM_USE_ACCELERATE_FRAMEWORK
    vec2_t out;
    GLM_vdsp(vadd, GLM_FCAST(v1), 1, GLM_FCAST(v2), 1, GLM_FCAST(out), 1, 2);
    return out;
#else
    return (vec2_t){ v1.x + v2.x, v1.y + v2.y };
#endif
}

GLM_INLINE vec2_t vec2_sub(const vec2_t v1, const vec2_t v2) {
#ifdef GLM_USE_ACCELERATE_FRAMEWORK
    vec2_t out;
    GLM_vdsp(vsub, GLM_FCAST(v2), 1, GLM_FCAST(v1), 1, GLM_FCAST(out), 1, 2);
    return out;
#else
    return (vec2_t){ v1.x - v2.x, v1.y - v2.y };
#endif
}

GLM_INLINE vec2_t vec2_mul(const vec2_t v1, const vec2_t v2) {
#ifdef GLM_USE_ACCELERATE_FRAMEWORK
    vec2_t out;
    GLM_vdsp(vmul, GLM_FCAST(v1), 1, GLM_FCAST(v2), 1, GLM_FCAST(out), 1, 2);
    return out;
#else
    return (vec2_t){ v1.x * v2.x, v1.y * v2.y };
#endif
}

GLM_INLINE vec2_t vec2_div(const vec2_t v1, const vec2_t v2) {
#ifdef GLM_USE_ACCELERATE_FRAMEWORK
    vec2_t out;
    GLM_vdsp(vdiv, GLM_FCAST(v2), 1, GLM_FCAST(v1), 1, GLM_FCAST(out), 1, 2);
    return out;
#else
    return (vec2_t){ v1.x / v2.x, v1.y / v2.y };
#endif
}

GLM_INLINE GLMFloat vec2_dot(const vec2_t v1, const vec2_t v2) {
#ifdef GLM_USE_ACCELERATE_FRAMEWORK
    GLMFloat out;
    GLM_vdsp(dotpr, GLM_FCAST(v1), 1, GLM_FCAST(v2), 1, &out, 2);
    return out;
#else
    return v1.x * v2.x + v1.y * v2.y;
#endif
}

GLM_INLINE GLMFloat vec2_magSquared(const vec2_t v) {
#ifdef GLM_USE_ACCELERATE_FRAMEWORK
    GLMFloat magnitudes[2];
    GLM_vdsp(vsq, GLM_FCAST(v), 1, magnitudes, 1, 2);
    return magnitudes[0] + magnitudes[1];
#else
    return (v.x * v.x) + (v.y * v.y);
#endif
}

GLM_INLINE GLMFloat vec2_mag(const vec2_t v) {
    return sqrtf(vec2_magSquared(v));
}

GLM_INLINE vec2_t vec2_normalize(const vec2_t v) {
    return vec2_scalarDiv(v, vec2_mag(v));
}

GLM_INLINE GLMFloat vec2_dist(const vec2_t v1, const vec2_t v2) {
    return vec2_mag(vec2_sub(v2, v1));
}

GLM_INLINE vec2_t vec2_scalarMul(const vec2_t v, GLMFloat s) {
#ifdef GLM_USE_ACCELERATE_FRAMEWORK
    vec2_t out;
    GLM_vdsp(vsmul, GLM_FCAST(v), 1, &s, GLM_FCAST(out), 1, 2);
    return out;
#else
    return  (vec2_t){ v.x * s, v.y * s };
#endif

}

GLM_INLINE vec2_t vec2_scalarDiv(const vec2_t v, GLMFloat s) {
#ifdef GLM_USE_ACCELERATE_FRAMEWORK
    vec2_t out;
    GLM_vdsp(vsdiv, GLM_FCAST(v), 1, &s, GLM_FCAST(out), 1, 2);
    return out;
#else
    return (vec2_t){ v.x / s, v.y / s };
#endif
}

GLM_INLINE vec2_t vec2_negate(const vec2_t v) {
#ifdef GLM_USE_ACCELERATE_FRAMEWORK
    vec2_t out;
    GLM_vdsp(vneg, GLM_FCAST(v), 1, GLM_FCAST(out), 1, 2);
    return out;
#else
    return vec2_scalarMul(v, -1.0);
#endif
}

GLM_INLINE vec2_t vec2_scalarAdd(const vec2_t v, GLMFloat s)
{
    return vec2_create(v.x+s, v.y+s);
}

GLM_INLINE vec2_t vec2_scalarSub(const vec2_t v, GLMFloat s)
{
    return vec2_create(v.x-s, v.y-s);
}

GLM_INLINE vec2_t vec2_floor(vec2_t v)
{
    vec2_t out = { floorf(v.x), floorf(v.y) };
    return out;
}


#pragma mark - ~ Vectors(3)
GLM_INLINE vec3_t vec3_create(GLMFloat x, GLMFloat y, GLMFloat z) {
    vec3_t out = { x, y, z };
    return out;
}

GLM_INLINE vec3_t vec3_add(const vec3_t v1, const vec3_t v2) {
#ifdef GLM_USE_ACCELERATE_FRAMEWORK
    vec3_t out;
    GLM_vdsp(vadd, GLM_FCAST(v1), 1, GLM_FCAST(v2), 1, GLM_FCAST(out), 1, 3);
    return out;
#else
    return (vec3_t){ v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
#endif
}

GLM_INLINE vec3_t vec3_sub(const vec3_t v1, const vec3_t v2) {
#ifdef GLM_USE_ACCELERATE_FRAMEWORK
    vec3_t out;
    GLM_vdsp(vsub, GLM_FCAST(v2), 1, GLM_FCAST(v1), 1, GLM_FCAST(out), 1, 3);
    return out;
#else
    return (vec3_t){ v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
#endif
}

GLM_INLINE vec3_t vec3_mul(const vec3_t v1, const vec3_t v2) {
#ifdef GLM_USE_ACCELERATE_FRAMEWORK
    vec3_t out;
    GLM_vdsp(vmul, GLM_FCAST(v1), 1, GLM_FCAST(v2), 1, GLM_FCAST(out), 1, 3);
    return out;
#else
    return (vec3_t){ v1.x * v2.x, v1.y * v2.y, v1.z * v2.z };
#endif
}

GLM_INLINE vec3_t vec3_div(const vec3_t v1, const vec3_t v2) {
#ifdef GLM_USE_ACCELERATE_FRAMEWORK
    vec3_t out;
    GLM_vdsp(vdiv, GLM_FCAST(v2), 1, GLM_FCAST(v1), 1, GLM_FCAST(out), 1, 3);
    return out;
#else
    return (vec3_t){ v1.x / v2.x, v1.y / v2.y, v1.z / v2.z };
#endif
}

GLM_INLINE GLMFloat vec3_dot(const vec3_t v1, const vec3_t v2) {
#ifdef GLM_USE_ACCELERATE_FRAMEWORK
    GLMFloat out;
    GLM_vdsp(dotpr, GLM_FCAST(v1), 1, GLM_FCAST(v2), 1, &out, 3);
    return out;
#else
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
#endif
}

GLM_INLINE GLMFloat vec3_magSquared(const vec3_t v) {
#ifdef GLM_USE_ACCELERATE_FRAMEWORK
    GLMFloat magnitudes[3];
    GLM_vdsp(vsq, GLM_FCAST(v), 1, magnitudes, 1, 3);
    return magnitudes[0] + magnitudes[1] + magnitudes[2];
#else
    return v.x * v.x + v.y * v.y + v.z * v.z;
#endif
}

GLM_INLINE GLMFloat vec3_mag(const vec3_t v) {
    return sqrtf(vec3_magSquared(v));
}

GLM_INLINE vec3_t vec3_normalize(const vec3_t v) {
    return vec3_scalarDiv(v, vec3_mag(v));
}

GLM_INLINE GLMFloat vec3_dist(const vec3_t v1, const vec3_t v2) {
    return vec3_mag(vec3_sub(v2, v1));
}

GLM_INLINE vec3_t vec3_scalarMul(const vec3_t v, GLMFloat s) {
#ifdef GLM_USE_ACCELERATE_FRAMEWORK
    vec3_t out;
    GLM_vdsp(vsmul, GLM_FCAST(v), 1, &s, GLM_FCAST(out), 1, 3);
    return out;
#else
    return (vec3_t){ v.x * s, v.y * s, v.z * s };
#endif
}

GLM_INLINE vec3_t vec3_scalarDiv(const vec3_t v, GLMFloat s) {
#ifdef GLM_USE_ACCELERATE_FRAMEWORK
    vec3_t out;
    GLM_vdsp(vsdiv, GLM_FCAST(v), 1, &s, GLM_FCAST(out), 1, 3);
    return out;
#else
    return (vec3_t){ v.x / s, v.y / s, v.z / s };
#endif
}

GLM_INLINE vec3_t vec3_scalarAdd(const vec3_t v, GLMFloat s)
{
    return vec3_create(v.x+s, v.y+s, v.z+s);
}

GLM_INLINE vec3_t vec3_scalarSub(const vec3_t v, GLMFloat s)
{
    return vec3_create(v.x-s, v.y-s, v.z-s);
}

GLM_INLINE vec3_t vec3_cross(const vec3_t v1, const vec3_t v2) {
    return (vec3_t){ v1.y * v2.z - v1.z * v2.y,
                     v1.z * v2.x - v1.x * v2.z,
                     v1.x * v2.y - v1.y * v2.x };
}

GLM_INLINE vec3_t vec3_negate(const vec3_t v) {
#ifdef GLM_USE_ACCELERATE_FRAMEWORK
    vec3_t out;
    GLM_vdsp(vneg, GLM_FCAST(v), 1, GLM_FCAST(out), 1, 3);
    return out;
#else
    return vec3_scalarMul(v, -1.0);
#endif
}

GLM_INLINE vec3_t vec3_floor(vec3_t v)
{
    vec3_t out = { floorf(v.x), floorf(v.y), floorf(v.z) };
    return out;
}


#pragma mark - ~ Vectors(4)
GLM_INLINE vec4_t vec4_create(GLMFloat x, GLMFloat y, GLMFloat z, GLMFloat w) {
    vec4_t out = { x, y, z, w };
    return out;
}

GLM_INLINE vec4_t vec4_add(const vec4_t v1, const vec4_t v2) {
#ifdef GLM_USE_ACCELERATE_FRAMEWORK
    vec4_t out;
    GLM_vdsp(vadd, GLM_FCAST(v1), 1, GLM_FCAST(v2), 1, GLM_FCAST(out), 1, 4);
    return out;
#else
    return (vec4_t){ v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w };
#endif
}

GLM_INLINE vec4_t vec4_sub(const vec4_t v1, const vec4_t v2) {
#ifdef GLM_USE_ACCELERATE_FRAMEWORK
    vec4_t out;
    GLM_vdsp(vsub, GLM_FCAST(v2), 1, GLM_FCAST(v1), 1, GLM_FCAST(out), 1, 4);
    return out;
#else
    return (vec4_t){ v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w };
#endif
}

GLM_INLINE vec4_t vec4_mul(const vec4_t v1, const vec4_t v2) {
#ifdef GLM_USE_ACCELERATE_FRAMEWORK
    vec4_t out;
    GLM_vdsp(vmul, GLM_FCAST(v1), 1, GLM_FCAST(v2), 1, GLM_FCAST(out), 1, 4);
    return out;
#else
    return (vec4_t){ v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w };
#endif
}

GLM_INLINE vec4_t vec4_div(const vec4_t v1, const vec4_t v2) {
#ifdef GLM_USE_ACCELERATE_FRAMEWORK
    vec4_t out;
    GLM_vdsp(vdiv, GLM_FCAST(v2), 1, GLM_FCAST(v1), 1, GLM_FCAST(out), 1, 4);
    return out;
#else
    return (vec4_t){ v1.x / v2.x, v1.y / v2.y, v1.z / v2.z, v1.w / v2.w };
#endif
}

GLM_INLINE GLMFloat vec4_dot(const vec4_t v1, const vec4_t v2) {
#ifdef GLM_USE_ACCELERATE_FRAMEWORK
    GLMFloat out;
    GLM_vdsp(dotpr, GLM_FCAST(v1), 1, GLM_FCAST(v2), 1, &out, 4);
    return out;
#else
    return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z) + (v1.w * v2.w);
#endif
}

GLM_INLINE GLMFloat vec4_magSquared(const vec4_t v) {
#ifdef GLM_USE_ACCELERATE_FRAMEWORK
    GLMFloat magnitudes[4];
    GLM_vdsp(vsq, GLM_FCAST(v), 1, magnitudes, 1, 4);
    return magnitudes[0] + magnitudes[1] + magnitudes[2] + magnitudes[3];
#else
    return (v.x * v.x) + (v.y * v.y) + (v.z * v.z) + (v.w * v.w);
#endif
}

GLM_INLINE GLMFloat vec4_mag(const vec4_t v) {
    return sqrtf(vec4_magSquared(v));
}

GLM_INLINE vec4_t vec4_normalize(const vec4_t v) {
    return vec4_scalarDiv(v, vec4_mag(v));
}

GLM_INLINE GLMFloat vec4_dist(const vec4_t v1, const vec4_t v2) {
    return vec4_mag(vec4_sub(v2, v1));
}

GLM_INLINE vec4_t vec4_scalarMul(const vec4_t v, GLMFloat s) {
#ifdef GLM_USE_ACCELERATE_FRAMEWORK
    vec4_t out;
    GLM_vdsp(vsmul, GLM_FCAST(v), 1, &s, GLM_FCAST(out), 1, 4);
    return out;
#else
    return (vec4_t){ v.x * s, v.y * s, v.z * s, v.w * s };
#endif
}

GLM_INLINE vec4_t vec4_scalarDiv(const vec4_t v, GLMFloat s) {
#ifdef GLM_USE_ACCELERATE_FRAMEWORK
    vec4_t out;
    GLM_vdsp(vsdiv, GLM_FCAST(v), 1, &s, GLM_FCAST(out), 1, 4);
    return out;
#else
    return (vec4_t){ v.x / s, v.y / s, v.z / s, v.w / s };
#endif
}

GLM_INLINE vec4_t vec4_scalarAdd(const vec4_t v, GLMFloat s)
{
    return vec4_create(v.x+s, v.y+s, v.z+s, v.w+s);
}

GLM_INLINE vec4_t vec4_scalarSub(const vec4_t v, GLMFloat s)
{
    return vec4_create(v.x-s, v.y-s, v.z-s, v.w-s);
}

GLM_INLINE vec4_t vec4_cross(const vec4_t v1, const vec4_t v2) {
    return (vec4_t){ v1.y * v2.z - v1.z * v2.y,
                     v1.z * v2.x - v1.x * v2.z,
                     v1.x * v2.y - v1.y * v2.x,
                     0.0 };
}

GLM_INLINE vec4_t vec4_negate(const vec4_t v) {
#ifdef GLM_USE_ACCELERATE_FRAMEWORK
    vec4_t out;
    GLM_vdsp(vneg, GLM_FCAST(v), 1, GLM_FCAST(out), 1, 4);
    return out;
#else
    return vec4_scalarMul(v, -1.0);
#endif
}

GLM_INLINE vec4_t vec4_floor(vec4_t v)
{
    vec4_t out = { floorf(v.x), floorf(v.y), floorf(v.z), floorf(v.w) };
    return out;
}

GLM_INLINE vec4_t vec4_premultiplyAlpha(vec4_t v)
{
    vec4_t out = { v.x * v.w, v.y * v.w, v.z * v.w, v.w };
    return out;
}
