//
//  raytracer.h
//  raytracer
//
//  Created by Krzysztof Gabis on 25.03.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#ifndef raytracer_raytracer_h
#define raytracer_raytracer_h

#include "geometry.h"
#include "object.h"
#include "array.h"

typedef void (*DrawFunction)(void *data, Color color, size_t x, size_t y);

typedef struct {
    
} RaytracerConfig;

typedef struct {
    Vector3 position;
    Vector3 direction;
    Vector3 planeCenter;
    Vector3 planeDirectionX;
    Vector3 planeDirectionY;
    double focalLength;
    double width;
    double height;
} Camera;

typedef struct {
    Camera camera;
    Color backgroundColor;
    Array objects; // Type-specific, no casting overhead
    Array lights;
    size_t resolutionX;
    size_t resolutionY;
} Raytracer;

void raytracer_init(Raytracer *rt, size_t resolutionX, size_t resolutionY);
void raytracer_loadDemo(Raytracer *rt);
void raytracer_addObject(Raytracer *rt, Object *object);
void raytracer_addObjectRange(Raytracer *rt, Object *objects, size_t len);
void raytracer_render(Raytracer *rt, DrawFunction draw, void *data);
void raytracer_dealloc(Raytracer *rt);

#endif
