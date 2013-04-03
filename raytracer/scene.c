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
    scene->ambientLight = light_make(VEC3_ZERO, VEC3_ZERO, COLOR_WHITE, 0.0);
    camera_init(&scene->camera, 250.0, 640, 480);
    scene->backgroundColor = COLOR_WHITE;
}

void scene_loadDemo(Scene *scene) {
    Object spheres[3];
    Object triangles[10];
    Vector3 vs[8];
    spheres[0] = object_initSphere(vec3_make(0, 45, 100), 25,
                                   material_make(COLOR_RED, 0.0));
    spheres[1] = object_initSphere(vec3_make(15, 10, 100), 30,
                                   material_make(COLOR_GREEN, 0.8));
    spheres[2] = object_initSphere(vec3_make(-15, 10, 100), 35,
                                   material_make(COLOR_BLUE, 0.0));
    Material sideWallMaterial1 = material_make(COLORS1_BLUE, 0.0);
    Material sideWallMaterial2 = material_make(COLORS1_GREEN, 0.0);
    Material ceilingMaterial = material_make(COLORS2_RED, 0.0);
    Material floorMaterial = material_make(COLORS2_RED, 0.0);
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
    scene->ambientLight = light_make(VEC3_ZERO, vec3_make(-1, -1, 1), COLOR_WHITE, 2.0);
    scene->ambientCoefficient = 0.7;
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