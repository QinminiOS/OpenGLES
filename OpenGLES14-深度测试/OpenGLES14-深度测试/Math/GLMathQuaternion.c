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

#include "GLMathQuaternion.h"


// Generates a unit quaternion
GLM_INLINE quat_t quat_createf(GLMFloat x, GLMFloat y, GLMFloat z, GLMFloat angle) {
    vec3_t vec = { x, y, z };
    return quat_createv(vec, angle);
}

GLM_INLINE quat_t quat_createv(vec3_t axis, GLMFloat angle) {
    quat_t out;
    out.vec = vec3_scalarMul(vec3_normalize(axis), sinf(angle/2.0));
    out.scalar = cosf(angle/2.0);

    return out;
}

// Generates the standard matrix representation of a quaternion
GLM_INLINE mat4_t quat_to_mat4(const quat_t q) {
    mat4_t out;
    // Matrix representation of a quaternion:
    // q = [a, b, c, w]
    //       [1 0 0 0]     [ 0 1 0  0]     [ 0  0 1 0]     [ 0 0  0 1]
    // q = a*[0 1 0 0] + b*[-1 0 0  0] + c*[ 0  0 0 1] + w*[ 0 0 -1 0]
    //       [0 0 1 0]     [ 0 0 0 -1]     [-1  0 0 0]     [ 0 1  0 0]
    //       [0 0 0 1]     [ 0 0 1  0]     [ 0 -1 0 0]     [-1 0  0 0]
    GLM_FCAST(out)[0] = GLM_FCAST(out)[5]  = GLM_FCAST(out)[10] = GLM_FCAST(out)[15] = q.vec.x; // a
    GLM_FCAST(out)[1] = GLM_FCAST(out)[14] = q.vec.y; // b
    GLM_FCAST(out)[4] = GLM_FCAST(out)[11] = -1.0*q.vec.y; // b
    GLM_FCAST(out)[2] = GLM_FCAST(out)[7]  = q.vec.z; // c
    GLM_FCAST(out)[8] = GLM_FCAST(out)[13] = -1.0*q.vec.z; // c
    GLM_FCAST(out)[3] = GLM_FCAST(out)[9]  = q.scalar;
    GLM_FCAST(out)[6] = GLM_FCAST(out)[12] = -1.0*q.scalar;

    out.m13 = out.m23 = 0.0;
    out.m30 = out.m31 = out.m32 = 0.0;
    return out;
}

// Converts the standard matrix representation of a quaternion back to it's source
GLM_INLINE quat_t mat4_to_quat(const mat4_t m) {
    // To convert back to a quat we just need to copy the first 3 items
    quat_t out = {0.0};
    memcpy(GLM_FCAST(out), GLM_FCAST(m), sizeof(quat_t));
    return out;
}

// Generates the orthogonal matrix representation(rotation matrix) of a quaternion
GLM_INLINE mat4_t quat_to_ortho(const quat_t q) {
    GLMFloat xx = q.vec.x*q.vec.x;
    GLMFloat xy = q.vec.x*q.vec.y;
    GLMFloat xz = q.vec.x*q.vec.z;
    GLMFloat xw = q.vec.x*q.scalar;
    GLMFloat yy = q.vec.y*q.vec.y;
    GLMFloat yz = q.vec.y*q.vec.z;
    GLMFloat yw = q.vec.y*q.scalar;
    GLMFloat zz = q.vec.z*q.vec.z;
    GLMFloat zw = q.vec.z*q.scalar;

    mat4_t out;
    GLM_FCAST(out)[0]  = 1.0 - 2.0*( yy + zz );
    GLM_FCAST(out)[4]  = 2.0 * ( xy - zw );
    GLM_FCAST(out)[8]  = 2.0 * ( xz + yw );
    GLM_FCAST(out)[1]  = 2.0 * ( xy + zw );
    GLM_FCAST(out)[5]  = 1 - 2.0 * ( xx + zz );
    GLM_FCAST(out)[9]  = 2.0 * ( yz - xw );
    GLM_FCAST(out)[2]  = 2.0 * ( xz - yw );
    GLM_FCAST(out)[6]  = 2.0 * ( yz + xw );
    GLM_FCAST(out)[10] = 1.0 - 2.0*( xx + yy );

    GLM_FCAST(out)[3] = GLM_FCAST(out)[7] = GLM_FCAST(out)[11] = GLM_FCAST(out)[12] = GLM_FCAST(out)[13] = GLM_FCAST(out)[14] = 0.0;
    GLM_FCAST(out)[15] = 1.0;

    return out;
}

GLM_INLINE quat_t ortho_to_quat(const mat4_t m) {
    quat_t out;
    GLMFloat trace = 1 + m.m00 + m.m11 + m.m22;
    GLMFloat s;
    // If the trace is too close to 0 we'll get serious distortion
    if(trace > 0.000001) {
        s = sqrtf(trace)*2.0;
        out.vec.x  = (GLM_FCAST(m)[9] - GLM_FCAST(m)[6]) / s;
        out.vec.y  = (GLM_FCAST(m)[2] - GLM_FCAST(m)[8]) / s;
        out.vec.z  = (GLM_FCAST(m)[4] - GLM_FCAST(m)[1]) / s;
        out.scalar = s / 4.0;
    }
    // If the trace is 0 then find which diagonal element has the greatest value
    else if(GLM_FCAST(m)[0] > GLM_FCAST(m)[5] && GLM_FCAST(m)[0] > GLM_FCAST(m)[10]) { // Col 0
        s = sqrtf(1.0 + GLM_FCAST(m)[0] - GLM_FCAST(m)[5] - GLM_FCAST(m)[10])*2.0;
        out.vec.x  = s / 4.0;
        out.vec.y  = (GLM_FCAST(m)[4] - GLM_FCAST(m)[1]) / s;
        out.vec.z  = (GLM_FCAST(m)[2] - GLM_FCAST(m)[8]) / s;
        out.scalar = (GLM_FCAST(m)[9] - GLM_FCAST(m)[6]) / s;
    } else if(GLM_FCAST(m)[5] > GLM_FCAST(m)[10]) { // Col 1
        s = sqrtf(1.0 + GLM_FCAST(m)[5] - GLM_FCAST(m)[0] - GLM_FCAST(m)[10])*2.0;
        out.vec.x  = (GLM_FCAST(m)[4] - GLM_FCAST(m)[1]) / s;
        out.vec.y  = s / 4.0;
        out.vec.z  = (GLM_FCAST(m)[9] - GLM_FCAST(m)[6]) / s;
        out.scalar = (GLM_FCAST(m)[2] - GLM_FCAST(m)[8]) / s;
    } else { // Col 2
        s = sqrtf(1.0 + GLM_FCAST(m)[10] - GLM_FCAST(m)[0] - GLM_FCAST(m)[5])*2.0;
        out.vec.x  = (GLM_FCAST(m)[2] - GLM_FCAST(m)[8]) / s;
        out.vec.y  = (GLM_FCAST(m)[9] - GLM_FCAST(m)[6]) / s;
        out.vec.z  = s / 4.0;
        out.scalar = (GLM_FCAST(m)[4] - GLM_FCAST(m)[1]) / s;
    }

    return out;
}

GLM_INLINE GLMFloat quat_magSquared(const quat_t q) {
    return vec4_magSquared(*(const vec4_t *)&q);
}

GLM_INLINE GLMFloat quat_mag(const quat_t q) {
    return vec4_mag(*(const vec4_t *)&q);
}

GLM_INLINE quat_t quat_computeW(quat_t q) {
    quat_t out = q;

    GLMFloat t = 1.0 - quat_magSquared(out);

    if(t < 0.0)
        out.scalar = 0.0;
    else
        out.scalar = -1*sqrtf(t);

    return out;
}

GLM_INLINE quat_t quat_normalize(quat_t q) {
    vec4_t out;
    memcpy(GLM_FCAST(out), GLM_FCAST(q), sizeof(quat_t));

    GLMFloat mag = quat_mag(*(const quat_t*)&out);
    // Normalize if the magnitude is > 0
    if (mag <= 0.0)
        return *(quat_t *)&out;
    GLMFloat normalizedMag = 1.0 / mag;
    out = vec4_scalarMul(out, normalizedMag);
    return *(quat_t *)&out;
}

GLM_INLINE quat_t quat_multQuat(const quat_t q1, const quat_t q2) {
    quat_t out;
    vec3_t va = vec3_cross(q1.vec, q2.vec);
    vec3_t vb = vec3_scalarMul(q1.vec, q2.scalar);
    vec3_t vc = vec3_scalarMul(q2.vec, q1.scalar);
    
    out.scalar = q1.scalar * q2.scalar - vec3_dot(q1.vec, q2.vec);
    out.vec = vec3_add(va, vec3_add(vb, vc));
    return quat_normalize(out);
}

GLM_INLINE vec4_t quat_rotatePoint(const quat_t q, const vec4_t v) {
    mat4_t rotationMatrix;
    rotationMatrix = quat_to_ortho(q);
    return vec4_mul_mat4(v, rotationMatrix);
}

GLM_INLINE vec3_t quat_rotateVec3(const quat_t q, const vec3_t v)
{
    vec4_t temp = quat_rotatePoint(q, (vec4_t){v.x,v.y,v.z,1});
    return  (vec3_t){ temp.x, temp.y, temp.z };
}

GLM_INLINE GLMFloat quat_dotProduct(const quat_t q1, const quat_t q2) {
    return vec4_dot(*(const vec4_t*)&q1, *(const vec4_t*)&q2);
}

GLM_INLINE quat_t quat_inverse(const quat_t q) {
    // Multiply the vector by -1 and leave the angle alone
    vec4_t out;
    out = vec4_scalarMul(*(vec4_t *)&q, -1.0);
    out.w = q.scalar;
    return *(quat_t *)&out;
}

GLM_INLINE quat_t quat_slerp(const quat_t q1, const quat_t q2, GLMFloat t) {
    // Return edge points without interpolating if the value is out of range
    if(t <= 0.0)
        return q1;
    else if(t >= 1.0)
        return q2;

    // Compute the cosine of the angle between the quaternions
    GLMFloat cosOmega = quat_dotProduct(q1, q2);
    // If the dot product is <0 then we use -q2 otherwise q2
    quat_t q2Prepared;
    if(cosOmega < 0.0) {
        vec4_t multiplied = vec4_scalarMul(*(vec4_t *)&q2, -1.0);
        q2Prepared = *(quat_t *)&multiplied;
        cosOmega *= -1.0;
    } else {
        q2Prepared = q2;
    }
    // Verify that the quaternions are in fact unit quaternions
    assert(cosOmega <= 1.0+GLM_EPSILON);

    GLMFloat k1, k2;
    // If the angle is tiny enough just use linear interpolation
    if(cosOmega > 0.9999f) {
        k1 = 1.0 - t;
        k2 = t;
    } else {
        GLMFloat sinOmega = sqrtf(1.0 - powf(cosOmega, 2));
        // Calculate the actual angle
        GLMFloat omega = atan2f(cosOmega, sinOmega);

        GLMFloat sinOmegaInverse = 1.0 / sinOmega;
        k1 = sin((1.0 - t) * omega) * sinOmegaInverse;
        k2 = sin(t * omega) * sinOmegaInverse;
    }
    quat_t out;
    out.vec = vec3_add(vec3_scalarMul(q1.vec, k1), vec3_scalarMul(q2Prepared.vec, k2));
    out.scalar = (k1 * q1.scalar) + (k2 * q2Prepared.scalar);

    return out;
}
