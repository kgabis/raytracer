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
    Vector3 position;
    Color color;
} Vertex3;

typedef struct {
    Vector3 center;
    double radius;
} Sphere;

typedef struct {
    Vector3 a, b, c;
} Triangle;



#endif
