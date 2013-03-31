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
    s.geometry.sphere.center = position;
    s.geometry.sphere.radius = radius;
    s.material = material;
    return s;
}

Object object_initTriangle(Vector3 a, Vector3 b, Vector3 c, Material material) {
    Object t;
    t.type = GTTriangle;
    t.geometry.triangle.a = a;
    t.geometry.triangle.b = b;
    t.geometry.triangle.c = c;
    t.material = material;
    return t;
}