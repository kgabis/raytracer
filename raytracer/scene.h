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
    double ambientCoefficient;
    Camera camera;
    Color backgroundColor;
} Scene;

void scene_init(Scene *scene, size_t width, size_t height);
void scene_loadSpheresDemo(Scene *scene);
void scene_loadTeapotDemo(Scene *scene);
void scene_loadSnowmanDemo(Scene *scene);
void scene_loadMesh(Scene *scene, const char *filename, Material meshMaterial);
void scene_AddLight(Scene *scene, Light *light);
void scene_AddLightRange(Scene *scene, Light *lights, size_t len);
void scene_addObject(Scene *scene, Object *object);
void scene_addObjectRange(Scene *scene, Object *objects, size_t len);
void scene_dealloc(Scene *scene);

#endif
