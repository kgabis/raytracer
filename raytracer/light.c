//
//  light.c
//  raytracer
//
//  Created by Krzysztof Gabis on 01.04.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#include "light.h"

Light light_make(Vector3 position, double intensity) {
    Light l;
    l.position = position, l.intensity = intensity;
    return l;
}

Vector3 light_getDirection(const Light *light, Vector3 point) {
    return vec3_unit(vec3_sub(point, light->position));
}

double light_getShade(const Light *light, Vector3 direction, Vector3 normal) {
    double shade = VEC3_DOT(normal, direction);
    if (shade < 0) {
        shade = 0;
    }
    return shade * light->intensity;
}

void light_moveLeftRight(Light *light) {
    static int direction = -10;
    static double range = 60;
    if (light->position.x <= -range) {
        direction = 10;
    } else if (light->position.x >= range){
        direction = -10;
    }
    light->position.z += direction;
}
