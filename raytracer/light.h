//
//  light.h
//  raytracer
//
//  Created by Krzysztof Gabis on 01.04.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#ifndef raytracer_light_h
#define raytracer_light_h

#include "vector.h"
#include "color.h"

typedef struct {
    Vector3 position;
    double intensity;
} Light;

Light light_make(Vector3 position, double intensity);
Vector3 light_getDirection(const Light *light, Vector3 point);
double light_getShade(const Light *light, Vector3 point, Vector3 normal);
void light_moveLeftRight(Light *light);

#endif
