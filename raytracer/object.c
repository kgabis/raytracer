//
//  object.c
//  raytracer
//
//  Created by Krzysztof Gabis on 30.03.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#include <stdio.h>
#include "object.h"

Object object_initSphere(Vector3 position, double radius, Material material) {
    Object s;
    s.type = GTSphere;
    s.geometry.sphere = sphere_make(position, radius);
    s.material = material;
    return s;
}

Object object_initTriangle(Vector3 a, Vector3 b, Vector3 c, Material material) {
    Object t;
    t.type = GTTriangle;
    t.geometry.triangle = triangle_make(a, b, c);
    t.material = material;
    return t;
}

Vector3 object_getNormalAtPoint(const Object *object, Vector3 point) {
    switch (object->type) {
        case GTTriangle:
            return object->geometry.triangle.normal;
        case GTSphere:
            return vec3_unit(vec3_sub(object->geometry.sphere.center, point));
        default:
            return VEC3_ZERO;
            break;
    }
}