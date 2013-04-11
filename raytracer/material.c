//
//  material.c
//  raytracer
//
//  Created by Krzysztof Gabis on 31.03.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#include "material.h"

Material material_make(Color color, double reflectivity, double reflectionNoise, double specularity) {
    Material m;
    m.color = color;
    m.reflectivity = reflectivity;
    m.specularity = specularity;
    m.reflectionNoise = reflectionNoise;
    return m;
}