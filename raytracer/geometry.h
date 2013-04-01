//
//  geometry.h
//  raytracer
//
//  Created by Krzysztof Gabis on 25.03.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#ifndef raytracer_geometry_h
#define raytracer_geometry_h

#include "vector.h"
#include "color.h"

typedef struct {
    Vector3 center;
    double radius;
} Sphere;

typedef struct {
    Vector3 a, b, c;
    Vector3 edges[2];
    Vector3 normal;
} Triangle;

enum GeometryType {
    GTSphere,
    GTTriangle
};

union Geometry {
    Sphere sphere;
    Triangle triangle;
};

Sphere sphere_make(Vector3 center, double radius);
Triangle triangle_make(Vector3 a, Vector3 b, Vector3 c);

#endif
