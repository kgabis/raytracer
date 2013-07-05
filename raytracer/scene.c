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
    array_init(&scene->surfaces, INIT_CAPACITY, sizeof(Surface));
    array_init(&scene->lights, INIT_CAPACITY, sizeof(Light));
    camera_init(&scene->camera, VEC3_ZERO, vec3_make(0, 0, 1), 430.0, width, height);
    scene->ambientCoefficient = 0.6;
    scene->backgroundColor = COLOR_WHITE;
}

void scene_loadSpheresDemo(Scene *scene) {
    Surface spheres[5];
    Surface triangles[12];
    Vector3 vs[12];
    Light lights[4];
    scene->ambientCoefficient = 0.7;
    camera_init(&scene->camera, vec3_make(0, 35, -70), vec3_make(0, 0, 1), 500.0,
                scene->camera.width, scene->camera.height);
    lights[0] = light_make(vec3_make(60, 80, -30), 1.0);
    lights[1] = light_make(vec3_make(-30, 65, -300), 0.3);
    spheres[1] = surface_initSphere(vec3_make(45, -20, 70), 20,
                                   material_make(COLOR_GREEN, 0.5, 0.05, 20.2));
    spheres[2] = surface_initSphere(vec3_make(-35, -25, 80), 15,
                                   material_make(COLOR_BLUE, 0.5, 0.02, 100));
    spheres[0] = surface_initSphere(vec3_make(10, -10, 110), 30,
                                   material_make(COLOR_RED, 0.5, 0.01, 40.0));
    spheres[3] = surface_initSphere(vec3_make(10, 40, 110), 25,
                                   material_make(COLOR_BLUE, 0.5, 0, 40));
    spheres[4] = surface_initSphere(vec3_make(10, 80, 110), 20,
                                   material_make(COLOR_GREEN, 0.5, 0, 40));
    Material sideWallMaterial1 = material_make(COLOR_WHITE, 0.0, 0, 40);
    Material sideWallMaterial2 = material_make(COLOR_WHITE, 0.0, 0, 40);
    Material ceilingMaterial = material_make(COLOR_WHITE, 0.4, 0.15, 40);
    Material floorMaterial = material_make(COLOR_WHITE, 0.4, 0.15, 40);
    vs[0] = vec3_make(-75, -40, 0);
    vs[1] = vec3_make(-75, -40, 150);
    vs[2] = vec3_make(75, -40, 0);
    vs[3] = vec3_make(75, -40, 150);
    vs[4] = vec3_make(-75, 110, 0);
    vs[5] = vec3_make(-75, 110, 150);
    vs[6] = vec3_make(75, 110, 0);
    vs[7] = vec3_make(75, 110, 150);
    vs[8] = vec3_make(-75, -40, -700);
    vs[9] = vec3_make(75, -40, -700);
    vs[10] = vec3_make(75, 110, -700);
    vs[11] = vec3_make(-75, 110, -700);
    //Floor
    triangles[0] = surface_initTriangle(vs[2], vs[1], vs[0], floorMaterial);
    triangles[1] = surface_initTriangle(vs[2], vs[3], vs[1], floorMaterial);
    //Left wall
    triangles[2] = surface_initTriangle(vs[0], vs[1], vs[4], sideWallMaterial1);
    triangles[3] = surface_initTriangle(vs[1], vs[5], vs[4], sideWallMaterial1);
    //Right wall
    triangles[4] = surface_initTriangle(vs[6], vs[3], vs[2], sideWallMaterial1);
    triangles[5] = surface_initTriangle(vs[6], vs[7], vs[3], sideWallMaterial1);
    //Ceiling
    triangles[6] = surface_initTriangle(vs[4], vs[5], vs[6], ceilingMaterial);
    triangles[7] = surface_initTriangle(vs[5], vs[7], vs[6], ceilingMaterial);
    //Back
    triangles[8] = surface_initTriangle(vs[3], vs[7], vs[5], sideWallMaterial2);
    triangles[9] = surface_initTriangle(vs[5], vs[1], vs[3], sideWallMaterial2);
    triangles[10] = surface_initTriangle(vs[8], vs[9], vs[10], sideWallMaterial1);
    triangles[11] = surface_initTriangle(vs[8], vs[10], vs[11], sideWallMaterial1);
    scene_addSurfaceRange(scene, spheres, 5);
    scene_addSurfaceRange(scene, triangles, 12);
    scene_AddLightRange(scene, lights, 2);
}

void scene_loadTeapotDemo(Scene *scene) {
    Material teapotMaterial = material_make(COLOR_WHITE, 0.2, 0, 20);
    scene_loadMesh(scene, "teapot.txt", teapotMaterial);
    
    scene->backgroundColor = COLORS1_BLUE;
    Light light = light_make(vec3_make(10, 10, -20), 1.0);
    scene_AddLight(scene, &light);
    scene->ambientCoefficient = 0.6;
    camera_init(&scene->camera, vec3_make(0, 5, -10), vec3_make(0, -0.4, 1), 800.0,
                scene->camera.width, scene->camera.height);
    
    Material screenMaterial = material_make(COLORS1_BLUE, 0, 0, 200);
    Vector3 vs[8];
    vs[0] = vec3_make(-500, -500, -30);
    vs[1] = vec3_make(-500, 500, -30);
    vs[2] = vec3_make(500, 500, -30);
    vs[3] = vec3_make(500, -500, -30);
    
    vs[4] = vec3_make(-500, -500, 5);
    vs[5] = vec3_make(-500, 500, 5);
    vs[6] = vec3_make(500, 500, 5);
    vs[7] = vec3_make(500, -500, 5);
    Surface triangles[2]; // Back wall
    triangles[0] = surface_initTriangle(vs[6], vs[5], vs[4], screenMaterial);
    triangles[1] = surface_initTriangle(vs[7], vs[6], vs[4], screenMaterial);
    scene_addSurfaceRange(scene, triangles, 2);
}

void scene_loadSnowmanDemo(Scene *scene) {
    Surface spheres[5];
    Surface triangles[12];
    Vector3 vs[12];
    Light lights[4];
    scene->ambientCoefficient = 0.7;
    camera_init(&scene->camera, vec3_make(0, 35, -70), vec3_make(0, 0, 1), 500.0,
                scene->camera.width, scene->camera.height);
    lights[0] = light_make(vec3_make(80, 20, -30), 1.0);
    lights[1] = light_make(vec3_make(60, 65, 30), 1.0);
    lights[2] = light_make(vec3_make(65, 60, 30), 1.0);
    lights[3] = light_make(vec3_make(65, 65, 30), 1.0);
    spheres[1] = surface_initSphere(vec3_make(45, -20, 70), 20,
                                   material_make(COLOR_GREEN, 0.5, 0, 20.2));
    spheres[2] = surface_initSphere(vec3_make(-35, -25, 80), 15,
                                   material_make(COLOR_BLUE, 0.5, 0, 100));
    spheres[0] = surface_initSphere(vec3_make(10, -10, 110), 30,
                                   material_make(COLOR_WHITE, 0.2, 0, 40.0));
    spheres[3] = surface_initSphere(vec3_make(10, 30, 110), 25,
                                   material_make(COLOR_WHITE, 0.2, 0, 40));
    spheres[4] = surface_initSphere(vec3_make(10, 60, 110), 20,
                                   material_make(COLOR_WHITE, 0.2, 0, 40));
    Material sideWallMaterial1 = material_make(COLORS1_BLUE, 0.0, 0, 400);
    Material sideWallMaterial2 = material_make(COLORS1_BLUE, 0.0, 0, 400);
    Material ceilingMaterial = material_make(COLORS1_BLUE, 0.3, 0, 400);
    Material floorMaterial = material_make(COLOR_WHITE, 0.3, 0, 40);
    vs[0] = vec3_make(-75, -40, 0);
    vs[1] = vec3_make(-75, -40, 150);
    vs[2] = vec3_make(75, -40, 0);
    vs[3] = vec3_make(75, -40, 150);
    vs[4] = vec3_make(-75, 110, 0);
    vs[5] = vec3_make(-75, 110, 150);
    vs[6] = vec3_make(75, 110, 0);
    vs[7] = vec3_make(75, 110, 150);
    vs[8] = vec3_make(-75, -40, -70);
    vs[9] = vec3_make(75, -40, -70);
    vs[10] = vec3_make(75, 110, -70);
    vs[11] = vec3_make(-75, 110, -70);
    //Floor
    triangles[0] = surface_initTriangle(vs[2], vs[1], vs[0], floorMaterial);
    triangles[1] = surface_initTriangle(vs[2], vs[3], vs[1], floorMaterial);
    //Left wall
    triangles[2] = surface_initTriangle(vs[0], vs[1], vs[4], sideWallMaterial1);
    triangles[3] = surface_initTriangle(vs[1], vs[5], vs[4], sideWallMaterial1);
    //Right wall
    triangles[4] = surface_initTriangle(vs[6], vs[3], vs[2], sideWallMaterial1);
    triangles[5] = surface_initTriangle(vs[6], vs[7], vs[3], sideWallMaterial1);
    //Ceiling
    triangles[6] = surface_initTriangle(vs[4], vs[5], vs[6], ceilingMaterial);
    triangles[7] = surface_initTriangle(vs[5], vs[7], vs[6], ceilingMaterial);
    //Back
    triangles[8] = surface_initTriangle(vs[3], vs[7], vs[5], sideWallMaterial2);
    triangles[9] = surface_initTriangle(vs[5], vs[1], vs[3], sideWallMaterial2);
    triangles[10] = surface_initTriangle(vs[8], vs[9], vs[10], sideWallMaterial1);
    triangles[11] = surface_initTriangle(vs[8], vs[10], vs[11], sideWallMaterial1);
    scene_addSurfaceRange(scene, spheres, 5);
    scene_addSurfaceRange(scene, triangles, 10);
    scene_AddLightRange(scene, lights, 1);
}

void scene_loadMesh(Scene *scene, const char *filename, Material meshMaterial) {
    Array mesh = mesh_load(filename, meshMaterial);
    array_addArray(&scene->surfaces, &mesh);    
}

void scene_AddLight(Scene *scene, Light *light) {
    array_add(&scene->lights, light);
}

void scene_AddLightRange(Scene *scene, Light *lights, size_t len) {
    for (size_t i = 0; i < len; i++) {
        scene_AddLight(scene, &lights[i]);
    }
}

void scene_addSurface(Scene *scene, Surface *surface) {
    array_add(&scene->surfaces, surface);
}

void scene_addSurfaceRange(Scene *scene, Surface *surfaces, size_t len) {
    for (size_t i = 0; i < len; i++) {
        scene_addSurface(scene, &surfaces[i]);
    }
}

void scene_dealloc(Scene *scene) {
    array_dealloc(&scene->surfaces);
    array_dealloc(&scene->lights);
}