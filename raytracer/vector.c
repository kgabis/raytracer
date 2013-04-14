//
//  vector.c
//  raytracer
//
//  Created by Krzysztof Gabis on 26.03.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#include <stdio.h>
#include <math.h>
#include <stdint.h>

#include "vector.h"
#include "utils.h"

static double invSqrt_1(double y) {
    double xhalf = ( double )0.5 * y;
    long long i = *( long long* )( &y );
    i = 0x5fe6ec85e7de30daLL - ( i >> 1 );//LL suffix for (long long) type for GCC
    y = *( double* )( &i );
    y = y * ( ( double )1.5 - xhalf * y * y );
    return y;
}

//static double invSqrt_2(double number) {
//    int64_t i;
//    double x2, y;
//    const double threehalfs = 1.5F;
//    
//    x2 = number * 0.5F;
//    y = number;
//    i = * ( int64_t * ) &y;
//    i = 0x5fe6ec85e7de30da - ( i >> 1 );
//    y = * ( double * ) &i;
//    y = y * ( threehalfs - ( x2 * y * y ) ); // 1st iteration
//    y = y * ( threehalfs - ( x2 * y * y ) ); // 2nd iteration, this can be removed
//    return y;
//}

Vector3 vec3_make(double x, double y, double z) {
    Vector3 v;
    v.x = x, v.y = y, v.z = z;
    return v;
}

Vector3 vec3_add(Vector3 a, Vector3 b){
    return vec3_make(a.x + b.x, a.y + b.y, a.z + b.z);
}

Vector3 vec3_add3(Vector3 a, Vector3 b, Vector3 c) {
    return vec3_make(a.x + b.x + c.x, a.y + b.y + c.y, a.z + b.z + c.z);
}

double  vec3_dot(Vector3 a, Vector3 b){
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

Vector3 vec3_sub(Vector3 a, Vector3 b){
    return vec3_make(a.x - b.x, a.y - b.y, a.z - b.z);
}

Vector3 vec3_multByVec3(Vector3 a, Vector3 b) {
    return vec3_make(a.x * b.x, a.y * b.y, a.z * b.z);
}

Vector3 vec3_mult(Vector3 v, double b) {
    return vec3_make(v.x * b, v.y * b, v.z * b);
}

Vector3 vec3_div(Vector3 v, double b) {
    return vec3_mult(v, 1.0 / b);
}

double vec3_length(Vector3 v) {
    return sqrt(SQUARE(v.x) + SQUARE(v.y) + SQUARE(v.z));
}

Vector3 vec3_unit(Vector3 v) {
    double is = invSqrt_1(SQUARE(v.x) + SQUARE(v.y) + SQUARE(v.z));
    return vec3_mult(v, is);
}

Vector3 vec3_cross(Vector3 a, Vector3 b) {
    Vector3 v;
    v.x = (a.y * b.z) - (b.y * a.z);
    v.y = (a.z * b.x) - (b.z * a.x);
    v.z = (a.x * b.y) - (b.x * a.y);
    return v;
}

Vector3 vec3_negate(Vector3 v) {
    return vec3_make(-v.x, -v.y, -v.z);
}
