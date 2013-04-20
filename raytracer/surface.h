//
//  surfaces.h
//  raytracer
//
//  Created by Krzysztof Gabis on 25.03.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#ifndef raytracer_surface_h
#define raytracer_surface_h

#include "geometry.h"
#include "material.h"

typedef struct {
    enum GeometryType type;
    union Geometry geometry;
    Material material;
} Surface;

Surface surface_initSphere(Vector3 position, double radius, Material material);
Surface surface_initTriangle(Vector3 a, Vector3 b, Vector3 c, Material material);
Vector3 surface_getNormalAtPoint(const Surface *surface, Vector3 point);
#endif
