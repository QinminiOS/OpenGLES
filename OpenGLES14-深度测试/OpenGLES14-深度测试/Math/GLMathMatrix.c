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

#include "GLMathMatrix.h"
#include "GLMathVector.h"
#include <math.h>
#ifdef GLM_USE_ACCELERATE_FRAMEWORK
#import <Accelerate/Accelerate.h>
#endif

GLM_INLINE mat3_t mat3_mul(const mat3_t m1, const mat3_t m2) {
#ifdef GLM_USE_ACCELERATE_FRAMEWORK
    mat3_t out;
    GLM_vdsp(mmul, GLM_FCAST(m2), 1, GLM_FCAST(m1), 1, GLM_FCAST(out), 1, 3, 3, 3);
    return out;
#else
    mat3_t out;
    out.m00 = m1.m00 * m2.m00 + m1.m10 * m2.m01 + m1.m20 * m2.m02;
    out.m10 = m1.m00 * m2.m10 + m1.m10 * m2.m11 + m1.m20 * m2.m12;
    out.m20 = m1.m00 * m2.m20 + m1.m10 * m2.m21 + m1.m20 * m2.m22;

    out.m01 = m1.m01 * m2.m00 + m1.m11 * m2.m01 + m1.m21 * m2.m02;
    out.m11 = m1.m01 * m2.m10 + m1.m11 * m2.m11 + m1.m21 * m2.m12;
    out.m21 = m1.m01 * m2.m20 + m1.m11 * m2.m21 + m1.m21 * m2.m22;

    out.m02 = m1.m02 * m2.m00 + m1.m12 * m2.m01 + m1.m22 * m2.m02;
    out.m12 = m1.m02 * m2.m10 + m1.m12 * m2.m11 + m1.m22 * m2.m12;
    out.m22 = m1.m02 * m2.m20 + m1.m12 * m2.m21 + m1.m22 * m2.m22;

    return out;
#endif
}

GLM_INLINE vec3_t vec3_mul_mat3(const vec3_t v, const mat3_t m) {
#ifdef GLM_USE_ACCELERATE_FRAMEWORK
    vec3_t out;
    GLM_vdsp(mmul, GLM_FCAST(v), 1, GLM_FCAST(m), 1, GLM_FCAST(out), 1, 1, 3, 3);
    return out;
#else
    return (vec3_t){
        m.m00 * v.x + m.m10 * v.y + m.m20 * v.z,
        m.m01 * v.x + m.m11 * v.y + m.m21 * v.z,
        m.m02 * v.x + m.m12 * v.y + m.m22 * v.z };
#endif
}

GLM_INLINE mat3_t mat3_inverse(const mat3_t m, bool *success_out) {
    if(success_out != NULL) *success_out = true;
#ifdef GLM_USE_ACCELERATE_FRAMEWORK
    mat3_t out;
    __CLPK_integer colsA, rowsA, colsB, rowsB, status;
    colsA = rowsA = colsB = rowsB = 3;
    __CLPK_integer pivotIndices[colsA];
    memcpy(GLM_FCAST(out), GLM_FCAST(GLMMat3_identity), sizeof(mat3_t));
    GLM_gesv(&colsA, &colsB, GLM_FCAST(m), &rowsA, pivotIndices, GLM_FCAST(out), &rowsB, &status);
    if(status != 0) {
        if(success_out != NULL) *success_out = false;
        return GLMMat3_zero;
    }
    return out;
#else
    mat3_t out;
    GLMFloat det = mat3_det(m);
    if(fabs(det) < 0.0005) {
        if(success_out != NULL) *success_out = false;
        return GLMMat3_zero;
    }
    out.m00 =    m.m11*m.m22 - m.m12*m.m21   / det;
    out.m01 = -( m.m01*m.m22 - m.m21*m.m02 ) / det;
    out.m02 =    m.m01*m.m12 - m.m11*m.m02   / det;
    out.m10 = -( m.m10*m.m22 - m.m12*m.m20 ) / det;
    out.m11 =    m.m00*m.m22 - m.m20*m.m02   / det;
    out.m12 = -( m.m00*m.m12 - m.m10*m.m02 ) / det;
    out.m20 =    m.m10*m.m21 - m.m20*m.m11   / det;
    out.m21 = -( m.m00*m.m21 - m.m20*m.m01 ) / det;
    out.m22 =    m.m00*m.m11 - m.m01*m.m10   / det;

    return out;
#endif
}

GLM_INLINE mat3_t mat3_transpose(const mat3_t m) {
#ifdef GLM_USE_ACCELERATE_FRAMEWORK
    mat3_t out;
    GLM_vdsp(mtrans, GLM_FCAST(m), 1, GLM_FCAST(out), 1, 3, 3);
    return out;
#else
    return (mat3_t) { m.m00, m.m10, m.m20,
                      m.m01, m.m11, m.m21,
                      m.m02, m.m12, m.m22 };
#endif
}

GLM_INLINE mat3_t mat4_extract_mat3(const mat4_t m) {
    mat3_t out = { m.m00, m.m01, m.m02,
                   m.m10, m.m11, m.m12,
                   m.m20, m.m21, m.m22 };
    return out;
}

GLM_INLINE GLMFloat mat3_det(const mat3_t m) {
    return m.m00   * ( m.m11*m.m22 - m.m21*m.m12 )
           - m.m01 * ( m.m10*m.m22 - m.m20*m.m12 )
           + m.m02 * ( m.m10*m.m21 - m.m20*m.m11 );
}

    
#pragma mark - 4x4

GLM_INLINE mat4_t mat4_mul(mat4_t m1, mat4_t m2) {
#ifdef GLM_USE_ACCELERATE_FRAMEWORK
    mat4_t out;
    GLM_vdsp(mmul, GLM_FCAST(m2), 1, GLM_FCAST(m1), 1, GLM_FCAST(out), 1, 4, 4, 4);
    return out;
#else
    mat4_t m;
    m.m00 = m1.m00 * m2.m00  + m1.m10 * m2.m01  + m1.m20 * m2.m02   + m1.m30 * m2.m03;
    m.m10 = m1.m00 * m2.m10  + m1.m10 * m2.m11  + m1.m20 * m2.m12   + m1.m30 * m2.m13;
    m.m20 = m1.m00 * m2.m20  + m1.m10 * m2.m21  + m1.m20 * m2.m22  + m1.m30 * m2.m23;
    m.m30 = m1.m00 * m2.m30 + m1.m10 * m2.m31 + m1.m20 * m2.m32  + m1.m30 * m2.m33;

    m.m01 = m1.m01 * m2.m00  + m1.m11 * m2.m01  + m1.m21 * m2.m02   + m1.m31 * m2.m03;
    m.m11 = m1.m01 * m2.m10  + m1.m11 * m2.m11  + m1.m21 * m2.m12   + m1.m31 * m2.m13;
    m.m21 = m1.m01 * m2.m20  + m1.m11 * m2.m21  + m1.m21 * m2.m22  + m1.m31 * m2.m23;
    m.m31 = m1.m01 * m2.m30 + m1.m11 * m2.m31 + m1.m21 * m2.m32  + m1.m31 * m2.m33;

    m.m02 = m1.m02 * m2.m00  + m1.m12 * m2.m01  + m1.m22 * m2.m02  + m1.m32 * m2.m03;
    m.m12 = m1.m02 * m2.m10  + m1.m12 * m2.m11  + m1.m22 * m2.m12  + m1.m32 * m2.m13;
    m.m22 = m1.m02 * m2.m20  + m1.m12 * m2.m21  + m1.m22 * m2.m22 + m1.m32 * m2.m23;
    m.m32 = m1.m02 * m2.m30 + m1.m12 * m2.m31 + m1.m22 * m2.m32 + m1.m32 * m2.m33;

    m.m03 = m1.m03 * m2.m00  + m1.m13 * m2.m01  + m1.m23 * m2.m02  + m1.m33 * m2.m03;
    m.m13 = m1.m03 * m2.m10  + m1.m13 * m2.m11  + m1.m23 * m2.m12  + m1.m33 * m2.m13;
    m.m23 = m1.m03 * m2.m20  + m1.m13 * m2.m21  + m1.m23 * m2.m22 + m1.m33 * m2.m23;
    m.m33 = m1.m03 * m2.m30 + m1.m13 * m2.m31 + m1.m23 * m2.m32 + m1.m33 * m2.m33;
    return m;
#endif
}

GLM_INLINE vec3_t vec3_mul_mat4(const vec3_t v, const mat4_t m, bool isPoint) {
    vec4_t temp = { v.x, v.y, v.z, isPoint ? 1.0 : 0.0 };
    vec4_t result = vec4_mul_mat4(temp, m);
    return vec3_create(result.x, result.y, result.z);
}

GLM_INLINE vec4_t vec4_mul_mat4(const vec4_t v, const mat4_t m) {
#ifdef GLM_USE_ACCELERATE_FRAMEWORK
    vec4_t out;
    GLM_vdsp(mmul, GLM_FCAST(v), 1, GLM_FCAST(m), 1, GLM_FCAST(out), 1, 1, 4, 4);
    return out;
#else
    return (vec4_t){ m.m00*v.x + m.m10*v.y + m.m20*v.z + m.m30*v.w,
                     m.m01*v.x + m.m11*v.y + m.m21*v.z + m.m31*v.w,
                     m.m02*v.x + m.m12 * v.y + m.m22 * v.z + m.m32*v.w,
                     m.m03*v.x + m.m13*v.y + m.m23*v.z + m.m33*v.w };
#endif
}

GLM_INLINE mat4_t mat4_inverse(const mat4_t m, bool *success_out) {
    mat4_t out = {0};
    if(success_out != NULL) *success_out = true;
#ifdef GLM_USE_ACCELERATE_FRAMEWORK
    __CLPK_integer colsA, rowsA, colsB, rowsB, status;
    colsA = rowsA = colsB = rowsB = 4;
    __CLPK_integer pivotIndices[colsA];
    memcpy(GLM_FCAST(out), GLM_FCAST(GLMMat4_identity), sizeof(mat4_t));
    GLM_gesv(&colsA, &colsB, GLM_FCAST(m), &rowsA, pivotIndices, GLM_FCAST(out), &rowsB, &status);
    if(status != 0) {
        if(success_out != NULL) *success_out = false;

        return GLMMat4_zero;
    }
#else
    GLMFloat det = mat4_det(m);
    mat3_t mtemp;
    int sign;
    if(fabs(det) < 0.0005) {
        if(success_out != NULL) *success_out = false;
        return GLMMat4_zero;
    }
    for(int i = 0; i < 4; i++ ) {
        for(int j = 0; j < 4; j++ ) {
            sign = 1 - ( (i +j) % 2 ) * 2;
            mtemp = _mat4_sub_mat3(m, i, j);
            GLM_FCAST(out)[i+j*4] = ( mat3_det(mtemp) * sign ) / det;
        }
    }
#endif
    return out;
}

GLM_INLINE mat4_t mat4_transpose(const mat4_t m) {
#ifdef GLM_USE_ACCELERATE_FRAMEWORK
    mat4_t out;
    GLM_vdsp(mtrans, GLM_FCAST(m), 1, GLM_FCAST(out), 1, 4, 4);
    return out;
#else
    return (mat4_t){ m.m00, m.m10, m.m20,  m.m30,
                     m.m01, m.m11, m.m21,  m.m31,
                     m.m02, m.m12, m.m22, m.m32,
                     m.m03, m.m13, m.m23, m.m33 };
#endif
}

GLM_INLINE GLMFloat mat4_det(mat4_t m)
{
    GLMFloat det, out = 0.0, i = 1;
    mat3_t subMtx;
    for(int n = 0; n < 4; n++, i *= -1) {
        subMtx = _mat4_sub_mat3(m, 0, n);
        det    = mat3_det(subMtx);
        out += GLM_FCAST(m)[n] * det * i;
    }
    return out;
}


// Returns a 3x3 submatrix at the offset i,j
GLM_INLINE mat3_t _mat4_sub_mat3(mat4_t m, int i, int j) {
    mat3_t out = {0};
    int si, sj;
    // loop through 3x3 submatrix
    for(int di = 0; di < 3; di ++ ) {
        for(int dj = 0; dj < 3; dj ++ ) {
            // map 3x3 element (destination) to 4x4 element (source)
            si = di + ( ( di >= i ) ? 1 : 0 );
            sj = dj + ( ( dj >= j ) ? 1 : 0 );
            // copy element
            GLM_FCAST(out)[di * 3 + dj] = GLM_FCAST(m)[si * 4 + sj];
        }
    }
    return out;
}

#ifdef __cplusplus
}
#endif
