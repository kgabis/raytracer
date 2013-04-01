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
    Color color;
    double intensity;
} Light;

#endif
