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
    Vector3 direction;
    Color color;
    double intensity;
} Light;

Light light_make(Vector3 position, Vector3 direction, Color color, double intensity);
double light_getAmbientShade(const Light *light, Vector3 planeNormal);
void light_moveLeftRight(Light *light);

#endif
