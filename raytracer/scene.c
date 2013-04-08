//
//  scene.c
//  raytracer
//
//  Created by Krzysztof Gabis on 01.04.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#include <stdio.h>
#include "scene.h"
#include "mesh.h"

#define INIT_CAPACITY 256

void scene_init(Scene *scene, size_t width, size_t height) {
    array_init(&scene->objects, INIT_CAPACITY, sizeof(Object));
    array_init(&scene->lights, INIT_CAPACITY, sizeof(Light));
    camera_init(&scene->camera, VEC3_ZERO, vec3_make(0, 0, 1), 430.0, width, height);
    scene->ambientCoefficient = 0.5;
    scene->backgroundColor = COLOR_WHITE;
}

void scene_loadDemo(Scene *scene) {
    Object spheres[3];
    Object triangles[12];
    Vector3 vs[12];
    Light lights[4];
    camera_init(&scene->camera, vec3_make(0, 35, -50), vec3_make(0, 0, 1), 430.0,
                scene->camera.width, scene->camera.height);
    lights[0] = light_make(vec3_make(0, 0, -30), 1.0);
    lights[1] = light_make(vec3_make(60, 65, 30), 1.0);
    lights[2] = light_make(vec3_make(65, 60, 30), 1.0);
    lights[3] = light_make(vec3_make(65, 65, 30), 1.0);
    spheres[0] = object_initSphere(vec3_make(0, 55, 110), 30,
                                   material_make(COLOR_RED, 0.5, 40.0));
    spheres[1] = object_initSphere(vec3_make(25, 10, 80), 20,
                                   material_make(COLOR_GREEN, 0.5, 20.2));
    spheres[2] = object_initSphere(vec3_make(-25, 10, 50), 15,
                                   material_make(COLOR_BLUE, 0.5, 100));
    Material sideWallMaterial1 = material_make(COLOR_WHITE, 0.0, 200);
    Material sideWallMaterial2 = material_make(COLOR_WHITE, 0.0, 200);
    Material ceilingMaterial = material_make(COLOR_WHITE, 0.0, 200);
    Material floorMaterial = material_make(COLOR_WHITE, 0.0, 200);
    vs[0] = vec3_make(-75, -40, 50);
    vs[1] = vec3_make(-75, -40, 150);
    vs[2] = vec3_make(75, -40, 50);
    vs[3] = vec3_make(75, -40, 150);
    vs[4] = vec3_make(-75, 110, 50);
    vs[5] = vec3_make(-75, 110, 150);
    vs[6] = vec3_make(75, 110, 50);
    vs[7] = vec3_make(75, 110, 150);
    vs[8] = vec3_make(-75, -40, -70);
    vs[9] = vec3_make(75, -40, -70);
    vs[10] = vec3_make(75, 110, -70);
    vs[11] = vec3_make(-75, 110, -70);
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
    triangles[10] = object_initTriangle(vs[8], vs[9], vs[10], sideWallMaterial1);
    triangles[11] = object_initTriangle(vs[8], vs[10], vs[11], sideWallMaterial1);
    scene_addObjectRange(scene, spheres, 3);
    scene_addObjectRange(scene, triangles, 12);
    scene_AddLightRange(scene, lights, 1);
}

void scene_loadMesh(Scene *scene, const char *filename) {
    array_dealloc(&scene->objects);
    Material meshMaterial = material_make(COLOR_WHITE, 0.0, 20);
    Array mesh = mesh_load(filename, meshMaterial);
    scene->objects = mesh;
    scene->backgroundColor = COLORS1_BLUE;
    Light light = light_make(vec3_make(10, 10, -30), 1.0);
    scene_AddLight(scene, &light);
    camera_init(&scene->camera, vec3_make(0, 1, -10), vec3_make(0, 0, 1), 600.0,
                scene->camera.width, scene->camera.height);
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