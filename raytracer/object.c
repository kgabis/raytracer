//
//  object.c
//  raytracer
//
//  Created by Krzysztof Gabis on 30.03.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#include <stdio.h>
#include "object.h"

Object object_initSphere(Vector3 position, double radius, Color color) {
    Object obj;
    obj.type = GTSphere;
    obj.geometry.sphere.center = position;
    obj.geometry.sphere.radius = radius;
    obj.material.color = color;
    return obj;
}