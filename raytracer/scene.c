//
//  scene.c
//  raytracer
//
//  Created by Krzysztof Gabis on 01.04.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#include <stdio.h>
#include "scene.h"

#define INIT_CAPACITY 256

void scene_init(Scene *scene) {
    array_init(&scene->objects, INIT_CAPACITY, sizeof(Object));
    array_init(&scene->lights, INIT_CAPACITY, sizeof(Light));
    camera_init(&scene->camera, vec3_make(0, 35, 0), vec3_make(0, 0, 1), 350.0, 640, 480);
    scene->backgroundColor = COLOR_WHITE;
}

void scene_loadDemo(Scene *scene) {
    Object spheres[3];
    Object triangles[10];
    Vector3 vs[8];
    Light lights[4];
    lights[0] = light_make(vec3_make(60, 60, 30), 1.0);
    lights[1] = light_make(vec3_make(60, 65, 30), 1.0);
    lights[2] = light_make(vec3_make(65, 60, 30), 1.0);
    lights[3] = light_make(vec3_make(65, 65, 30), 1.0);
    spheres[0] = object_initSphere(vec3_make(0, 45, 60), 10,
                                   material_make(COLOR_RED, 0.5));
    spheres[1] = object_initSphere(vec3_make(25, 10, 80), 20,
                                   material_make(COLOR_GREEN, 0.5));
    spheres[2] = object_initSphere(vec3_make(-25, 10, 110), 20,
                                   material_make(COLOR_BLUE, 0.5));
    Material sideWallMaterial1 = material_make(COLOR_WHITE, 0.0);
    Material sideWallMaterial2 = material_make(COLOR_WHITE, 0.0);
    Material ceilingMaterial = material_make(COLOR_WHITE, 0.0);
    Material floorMaterial = material_make(COLOR_WHITE, 0.0);
    vs[0] = vec3_make(-75, -40, 50);
    vs[1] = vec3_make(-75, -40, 150);
    vs[2] = vec3_make(75, -40, 50);
    vs[3] = vec3_make(75, -40, 150);
    vs[4] = vec3_make(-75, 110, 50);
    vs[5] = vec3_make(-75, 110, 150);
    vs[6] = vec3_make(75, 110, 50);
    vs[7] = vec3_make(75, 110, 150);
    //Floor
    triangles[0] = object_initTriangle(vs[2], vs[1], vs[0], floorMaterial);
    triangles[1] = object_initTriangle(vs[2], vs[3], vs[1], floorMaterial);
    //Left wall
    triangles[2] = object_initTriangle(vs[0], vs[1], vs[4], sideWallMaterial1);
    triangles[3] = object_initTriangle(vs[1], vs[5], vs[4], sideWallMaterial1);
    //Right wall
    triangles[4] = object_initTriangle(vs[6], vs[3], vs[2], sideWallMaterial1);
    triangles[5] = object_initTriangle(vs[6], vs[7], vs[3], sideWallMaterial1);
    //Ceiling
    triangles[6] = object_initTriangle(vs[4], vs[5], vs[6], ceilingMaterial);
    triangles[7] = object_initTriangle(vs[5], vs[7], vs[6], ceilingMaterial);
    //Back
    triangles[8] = object_initTriangle(vs[3], vs[7], vs[5], sideWallMaterial2);
    triangles[9] = object_initTriangle(vs[5], vs[1], vs[3], sideWallMaterial2);
    scene_addObjectRange(scene, spheres, 3);
    scene_addObjectRange(scene, triangles, 10);
    scene_AddLightRange(scene, lights, 1);
    scene->ambientCoefficient = 0.6;
}

void scene_AddLight(Scene *scene, Light *light) {
    array_add(&scene->lights, light);
}

void scene_AddLightRange(Scene *scene, Light *lights, size_t len) {
    for (size_t i = 0; i < len; i++) {
        scene_AddLight(scene, &lights[i]);
    }
}

void scene_addObject(Scene *scene, Object *object) {
    array_add(&scene->objects, object);
}

void scene_addObjectRange(Scene *scene, Object *objects, size_t len) {
    for (size_t i = 0; i < len; i++) {
        scene_addObject(scene, &objects[i]);
    }
}

void scene_dealloc(Scene *scene) {
    array_dealloc(&scene->objects);
    array_dealloc(&scene->lights);
}