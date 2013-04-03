//
//  light.c
//  raytracer
//
//  Created by Krzysztof Gabis on 01.04.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#include "light.h"

Light light_make(Vector3 position, Vector3 direction, Color color, double intensity) {
    Light l;
    l.position = position;
    l.direction = vec3_unit(direction);
    l.color = color;
    l.intensity = intensity;
    return l;
}

double light_getAmbientShade(const Light *light, Vector3 planeNormal) {
    double shade = VEC3_DOT(planeNormal, light->direction);
    if (shade < 0) {
        shade = 0;
    }
    return shade * light->intensity;
}

void light_moveLeftRight(Light *light) {
    static int direction = -1;
    static double range = 0.8;
    if (light->direction.x <= -range) {
        direction = 1;
    } else if (light->direction.x >= range){
        direction = -1;
    }
    light->direction = vec3_unit(vec3_add(light->direction, vec3_make(0.1 * direction, 0, 0)));
}