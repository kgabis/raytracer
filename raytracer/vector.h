//
//  vector.h
//  raytracer
//
//  Created by Krzysztof Gabis on 26.03.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#ifndef raytracer_vector_h
#define raytracer_vector_h

#define VEC3_ZERO vec3_make(0.0, 0.0, 0.0)

// Use macros only for critical secions of code
#define VEC3_ADD(out, a, b)    \
            (out).x = (a).x + (b).x; \
            (out).y = (a).y + (b).y; \
            (out).z = (a).z + (b).z;

#define VEC3_ADD3(out, a, b, c)      \
            (out).x = (a).x + (b).x + (c).x; \
            (out).y = (a).y + (b).y + (c).x; \
            (out).z = (a).z + (b).z + (c).x;

#define VEC3_SUB(out, a, b)    \
            (out).x = (a).x - (b).x; \
            (out).y = (a).y - (b).y; \
            (out).z = (a).z - (b).z;

#define VEC3_DOT(a, b) ((a).x * (b).x + (a).y * (b).y + (a).z * (b).z)

#define VEC3_CROSS(out, a, b)          \
            (out).x=((a).y * (b).z) - ((b).y * (a).z); \
            (out).y=((a).z * (b).x) - ((b).z * (a).x); \
            (out).z=((a).x * (b).y) - ((b).x * (a).y);

#define VEC3_MULT(out, a, b) \
            (out).x = (a).x * b; \
            (out).y = (a).y * b; \
            (out).z = (a).z * b

#define VEC3_DIV(out, a, b)  \
            (out).x = (a).x / b; \
            (out).y = (a).y / b; \
            (out).z = (a).z / b; 

#define VEC3_MULTBYVEC3(out, a, b) \
            (out).x = (a).x * (b).x;     \
            (out).y = (a).y * (b).y;     \
            (out).z = (a).z * (b).z;

typedef struct {
    double x, y, z;
} Vector3;

Vector3 vec3_make(double x, double y, double z);
Vector3 vec3_add(Vector3 a, Vector3 b);
Vector3 vec3_add3(Vector3 a, Vector3 b, Vector3 c);
double  vec3_dot(Vector3 a, Vector3 b);
Vector3 vec3_sub(Vector3 a, Vector3 b);
Vector3 vec3_multByVec3(Vector3 a, Vector3 b);
Vector3 vec3_mult(Vector3 v, double b);
Vector3 vec3_div(Vector3 v, double b);
double  vec3_length(Vector3 v);
Vector3 vec3_unit(Vector3 v);
Vector3 vec3_cross(Vector3 a, Vector3 b);
Vector3 vec3_negate(Vector3 v);

#endif
