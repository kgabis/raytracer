//
//  vector.h
//  raytracer
//
//  Created by Krzysztof Gabis on 26.03.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#ifndef raytracer_vector_h
#define raytracer_vector_h

#define VECTOR3_ZERO vec3_make(0.0, 0.0, 0.0)

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

#endif
