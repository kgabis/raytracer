//
//  objects.h
//  raytracer
//
//  Created by Krzysztof Gabis on 25.03.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#ifndef raytracer_object_h
#define raytracer_object_h

#include "geometry.h"
#include "material.h"

enum GeometryType {
    GTSphere,
    GTTriangle
};

union Geometry {
    Sphere sphere;
    Triangle triangle;
};

typedef struct {
    enum GeometryType type;
    union Geometry geometry;
    Material material;
} Object;

Object object_initSphere(Vector3 position, double radius, Color color);

#endif
