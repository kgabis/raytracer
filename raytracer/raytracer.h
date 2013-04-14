//
//  raytracer.h
//  raytracer
//
//  Created by Krzysztof Gabis on 25.03.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#ifndef raytracer_raytracer_h
#define raytracer_raytracer_h

#include <stdlib.h>
#include "geometry.h"
#include "scene.h"

typedef void (*DrawFunction)(void *data, Color color, size_t x, size_t y);

typedef struct {
    Scene scene;
    size_t resolutionX;
    size_t resolutionY;
} Raytracer;

void raytracer_init(Raytracer *rt, size_t resolutionX, size_t resolutionY);
void raytracer_render(Raytracer *rt, DrawFunction draw, void *data);
void raytracer_dealloc(Raytracer *rt);

#endif
