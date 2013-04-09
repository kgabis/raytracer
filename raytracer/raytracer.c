//
//  raytracer.c
//  raytracer
//
//  Created by Krzysztof Gabis on 29.03.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//
#include <stdio.h>

#include "raytracer.h"
#include "ray.h"

void raytracer_init(Raytracer *rt, size_t resolutionX, size_t resolutionY) {
    scene_init(&rt->scene, resolutionX, resolutionY);
    rt->resolutionX = resolutionX;
    rt->resolutionY = resolutionY;
}

void raytracer_render(Raytracer *rt, DrawFunction draw, void *data) {
//    static int cameraStatus = 0;
//    if (cameraStatus) {
//        rt->scene.camera.position.x += 3;
//        cameraStatus = 0;
//    } else {
//        rt->scene.camera.position.x -= 3;
//        cameraStatus = 1;
//    }
    camera_setup(&rt->scene.camera);
    for (size_t x = 0; x < rt->resolutionX; x++) {
        for (size_t y = 0; y < rt->resolutionY; y++) {
            Ray ray = ray_makeForPixel(&rt->scene.camera, x, y);
            Color color = ray_trace(ray, &rt->scene);
            draw(data, color, x, y);
        }
    }
}

void raytracer_dealloc(Raytracer *rt) {
    scene_dealloc(&rt->scene);
}
