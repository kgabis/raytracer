//
//  scene.h
//  raytracer
//
//  Created by Krzysztof Gabis on 01.04.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#ifndef raytracer_scene_h
#define raytracer_scene_h

#include "array.h"
#include "camera.h"
#include "color.h"
#include "object.h"
#include "light.h"

typedef struct {
    Array objects;
    Array lights;
    Light ambientLight;
    double ambientCoefficient;
    Camera camera;
    Color backgroundColor;
} Scene;

void scene_init(Scene *scene);
void scene_loadDemo(Scene *scene);
void scene_addObject(Scene *scene, Object *object);
void scene_addObjectRange(Scene *scene, Object *objects, size_t len);
void scene_dealloc(Scene *scene);

#endif
