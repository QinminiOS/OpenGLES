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
#include "GLMathUtilities.h"
#include <math.h>

GLM_INLINE GLMFloat degToRad(GLMFloat degrees) { return degrees * (M_PI / 180.0); }
GLM_INLINE GLMFloat radToDeg(GLMFloat radians) { return radians * (180.0 / M_PI); }

#ifdef ANDROID
GLM_INLINE GLMFloat log2f(GLMFloat f)
{
    return logf(f) * (GLMFloat) (1.0 / M_LN2);
}
#endif

GLM_INLINE GLMFloat fastPow(GLMFloat x,GLMFloat y)
{
#ifdef GLM_USE_DOUBLE
    return exp2(y*log2(x));
#else
    return exp2f(y*log2f(x));
#endif
}

GLM_INLINE bool GLMFloatArr_equals(const GLMFloat *a1, const GLMFloat *a2, unsigned int len) {
    for(int i = 0; i < len; ++i) {
        if(a1[i] != a2[i])
            return false;
    }
    return true;
}
GLM_INLINE bool vec2_equals(const vec2_t v1, const vec2_t v2) {
    return GLMFloatArr_equals(GLM_FCAST(v1), GLM_FCAST(v2), 2);
}
GLM_INLINE bool vec3_equals(const vec3_t v1, const vec3_t v2) {
    return GLMFloatArr_equals(GLM_FCAST(v1), GLM_FCAST(v2), 3);
}
GLM_INLINE bool vec4_equals(const vec4_t v1, const vec4_t v2) {
    return GLMFloatArr_equals(GLM_FCAST(v1), GLM_FCAST(v2), 4);
}
GLM_INLINE bool mat4_equals(const mat4_t m1, const mat4_t m2) {
    return GLMFloatArr_equals(GLM_FCAST(m1), GLM_FCAST(m2), 16);
}
GLM_INLINE bool quat_equals(const quat_t q1, const quat_t q2) {
    return GLMFloatArr_equals(GLM_FCAST(q1), GLM_FCAST(q2), 4);
}
