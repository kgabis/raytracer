//
//  camera.h
//  raytracer
//
//  Created by Krzysztof Gabis on 01.04.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#ifndef raytracer_camera_h
#define raytracer_camera_h

#include "vector.h"

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

void camera_init(Camera *cam, Vector3 position, Vector3 direction, double focalLength, double width, double height);
void camera_setup(Camera *c);

#endif
