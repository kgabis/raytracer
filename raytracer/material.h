//
//  Material.h
//  raytracer
//
//  Created by Krzysztof Gabis on 25.03.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#ifndef raytracer_material_h
#define raytracer_material_h

#include "color.h"

typedef struct {
    Color color;
    double reflectivity;
    double specularity;
    double reflectionNoise;
} Material;

Material material_make(Color color, double reflectivity, double reflectionNoise, double specularity);

#endif
