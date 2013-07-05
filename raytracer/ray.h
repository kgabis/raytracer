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
#include "surface.h"
#include "scene.h"
#include "camera.h"

typedef struct {
    Vector3 origin;
    Vector3 direction;
} Ray;

Ray ray_make(Vector3 origin, Vector3 direction);
Ray ray_makeForPixel(const Camera *c, size_t x, size_t y);
Color ray_trace(const Ray *ray, const Scene *scene);

#endif
