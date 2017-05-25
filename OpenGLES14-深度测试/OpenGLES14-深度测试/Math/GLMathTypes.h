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

#ifndef GL_MATH_TYPES_H
#define GL_MATH_TYPES_H

#ifdef GLM_USE_INLINE
    #define GLM_INLINE static __inline__
#else
    #define GLM_INLINE
#endif

#define GLM_FCAST(x) ((GLMFloat *)(&x))

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __SCRIPTINGBRIDGE__
#define GLM_UNIONS
#endif

#pragma mark - Types

// Scalar
#ifdef GLM_USE_DOUBLE
    typedef double GLMFloat;
    #define GLM_vdsp(fun, ...) vDSP_##fun##D(__VA_ARGS__)
#else
    typedef float GLMFloat;
    #define GLM_vdsp(fun, ...) vDSP_##fun(__VA_ARGS__)
#endif

// Vectors
#ifdef GLM_UNIONS
union _vec2_t {
    GLMFloat f[2];
    struct { GLMFloat x; GLMFloat y; };
    struct { GLMFloat w; GLMFloat h; };
    struct { GLMFloat u; GLMFloat v; };

};
typedef union _vec2_t vec2_t;
#else
typedef struct _vec2_t { GLMFloat x; GLMFloat y; } vec2_t;
#endif

#ifdef GLM_UNIONS
union _vec3_t {
    GLMFloat f[3];
    struct { GLMFloat x; GLMFloat y; GLMFloat z; };
    struct { GLMFloat r; GLMFloat g; GLMFloat b; };
    struct { vec2_t xy; GLMFloat andY; };
};
typedef union _vec3_t vec3_t;
#else
typedef struct _vec3_t { GLMFloat x; GLMFloat y; GLMFloat z; } vec3_t;
#endif

#ifdef GLM_UNIONS
union _vec4_t {
    GLMFloat f[4];
    struct { GLMFloat x; GLMFloat y; GLMFloat z; GLMFloat w; };
    struct { GLMFloat r; GLMFloat g; GLMFloat b; GLMFloat a; };
    struct { vec3_t xyz; GLMFloat andW; };
};
typedef union _vec4_t vec4_t;
#else
typedef struct _vec4_t { GLMFloat x; GLMFloat y; GLMFloat z; GLMFloat w; } vec4_t;
#endif

// Matrices
#ifdef GLM_UNIONS
union _mat3_t {
    GLMFloat f[9];
    struct {
        GLMFloat m00, m01, m02;
        GLMFloat m10, m11, m12;
        GLMFloat m20, m21, m22;
    };
};
typedef union _mat3_t mat3_t;
#else
typedef struct _mat3_t {
    GLMFloat m00, m01, m02;
    GLMFloat m10, m11, m12;
    GLMFloat m20, m21, m22;
} mat3_t;
#endif

#ifdef GLM_UNIONS
union _mat4_t {
    GLMFloat f[16];
    struct {
        GLMFloat m00, m01, m02, m03;
        GLMFloat m10, m11, m12, m13;
        GLMFloat m20, m21, m22, m23;
        GLMFloat m30, m31, m32, m33;
    };
};
typedef union _mat4_t mat4_t;
#else
typedef struct _mat4_t {
    GLMFloat m00, m01, m02, m03;
    GLMFloat m10, m11, m12, m13;
    GLMFloat m20, m21, m22, m23;
    GLMFloat m30, m31, m32, m33;
} mat4_t;
#endif

// Quaternions
#ifdef GLM_UNIONS
union _quat_t {
    GLMFloat f[4];
    struct { GLMFloat x; GLMFloat y; GLMFloat z; GLMFloat w; };
    struct { vec3_t vec; GLMFloat scalar; };
};
typedef union _quat_t quat_t;
#else
typedef struct _quat_t { vec3_t vec; GLMFloat scalar; } quat_t;
#endif

// Matrix stacks
struct _matrix_stack_t {
    mat4_t *items;
    unsigned int capacity;
    unsigned int count;
};
typedef struct _matrix_stack_t matrix_stack_t;

// Bezier curves
#ifdef GLM_UNIONS
union _bezier_t {
    GLMFloat f[12];
    vec3_t controlPoints[4];
    vec3_t cp[4];
};
typedef union _bezier_t bezier_t;
#else
typedef struct _bezier_t { vec3_t controlPoints[4]; } bezier_t;
#endif

enum _bezierAxis {
    kBezierAxisX = 0,
    kBezierAxisY,
    kBezierAxisZ
};
typedef enum _bezierAxis bezierAxis_t;

#pragma mark - Constants

// Zero vectors
extern const vec2_t GLMVec2_zero;
extern const vec3_t GLMVec3_zero;
extern const vec4_t GLMVec4_zero;

extern const mat3_t GLMMat3_identity;
extern const mat3_t GLMMat3_zero;
extern const mat4_t GLMMat4_identity;
extern const mat4_t GLMMat4_zero;

#ifdef __cplusplus
}
#endif
#endif
