//
//  surface.c
//  raytracer
//
//  Created by Krzysztof Gabis on 30.03.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#include <stdio.h>
#include "surface.h"

Surface surface_initSphere(Vector3 position, double radius, Material material) {
    Surface s;
    s.type = GTSphere;
    s.geometry.sphere = sphere_make(position, radius);
    s.material = material;
    return s;
}

Surface surface_initTriangle(Vector3 a, Vector3 b, Vector3 c, Material material) {
    Surface t;
    t.type = GTTriangle;
    t.geometry.triangle = triangle_make(a, b, c);
    t.material = material;
    return t;
}

Vector3 surface_getNormalAtPoint(const Surface *surface, Vector3 point) {
    switch (surface->type) {
        case GTTriangle:
            return surface->geometry.triangle.normal;
        case GTSphere:
            return vec3_unit(vec3_sub(surface->geometry.sphere.center, point));
        default:
            return VEC3_ZERO;
            break;
    }
}