//
//  raytracer.c
//  raytracer
//
//  Created by Krzysztof Gabis on 29.03.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//
// 17.08.2013 15:10 - O3 
//Teapot:
//Seconds per frame: 97.572769
// 17.08.2013 15:37 - O3 OMP 
//Teapot:
//Seconds per frame: 54.484337
//Seconds per frame: 54.581848
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
    camera_setup(&rt->scene.camera);
    #pragma omp parallel for
    for (int x = 0; x < rt->resolutionX; x++) {
        for (int y = 0; y < rt->resolutionY; y++) {
            Ray ray = ray_makeForPixel(&rt->scene.camera, x, y);
            Color color = ray_trace(&ray, &rt->scene);
            draw(data, color, x, y);
        }
    }
}

void raytracer_dealloc(Raytracer *rt) {
    scene_dealloc(&rt->scene);
}
