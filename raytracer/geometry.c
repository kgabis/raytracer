//
//  geometry.c
//  raytracer
//
//  Created by Krzysztof Gabis on 31.03.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#include <stdio.h>

#include "geometry.h"

Sphere sphere_make(Vector3 center, double radius) {
    Sphere s = { .center = center, .radius = radius };
    return s;
}

Triangle triangle_make(Vector3 a, Vector3 b, Vector3 c) {
    Triangle t = { .a = a, .b = b, .c = c };
    VEC3_SUB(t.edges[0], t.b, t.a);
    VEC3_SUB(t.edges[1], t.c, t.a);
    t.normal = vec3_unit(vec3_cross(vec3_sub(t.b, t.a), vec3_sub(t.c, t.a)));    
    return t;
}
