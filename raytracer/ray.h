//
//  ray.h
//  raytracer
//
//  Created by Krzysztof Gabis on 01.04.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#ifndef raytracer_ray_h
#define raytracer_ray_h

#include "vector.h"
#include "color.h"
#include "object.h"
#include "scene.h"


typedef struct {
    Vector3 origin;
    Vector3 direction;
} Ray;

typedef struct {
    int hit;
    Color color;
    Object *object;
    double distance;
} TracingResult;

Ray ray_makeForPixel(Camera *c, size_t x, size_t y);
TracingResult ray_trace(Ray ray, Scene *scene);
TracingResult ray_checkIntersection(Ray ray, Object *object);
TracingResult ray_checkSphereIntersection_1(Ray ray, Sphere sphere);
TracingResult ray_checkSphereIntersection_2(Ray ray, Sphere sphere);
TracingResult ray_checkTriangleIntersection(Ray ray, Triangle triangle);


#endif
