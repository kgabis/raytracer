//
//  camera.c
//  raytracer
//
//  Created by Krzysztof Gabis on 01.04.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#include <stdio.h>
#include "camera.h"

void camera_init(Camera *cam, Vector3 position, Vector3 direction, double focalLength, double width, double height) {
    cam->position = position;
    cam->direction = vec3_unit(direction);
    cam->focalLength = focalLength;
    cam->width = width;
    cam->height = height;
}

// See: http://people.cs.clemson.edu/~dhouse/courses/405/notes/raycamera.pdf
void camera_setup(Camera *c) {
    Vector3 x_c = c->position;
    Vector3 u_c = c->direction;
    double z_p = c->focalLength;
    Vector3 v_up = vec3_make(0.0, -1.0, 0.0);
    Vector3 c_0 = vec3_add(x_c, vec3_mult(u_c, z_p));
    Vector3 u_x = vec3_unit(vec3_cross(u_c, v_up));
    Vector3 u_y = vec3_cross(vec3_mult(u_c, -1.0), u_x);
    c->planeCenter = c_0;
    c->planeDirectionX = u_x;
    c->planeDirectionY = u_y;
    //    Vector3 u_z = vec3_mult(u_c, -1.0); // Normal to the view plane
}