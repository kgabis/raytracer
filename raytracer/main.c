//
//  main.c
//  raytracer
//
//  Created by Krzysztof Gabis on 13.04.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#include <stdio.h>
#include <string.h>

#include "raytracer.h"
#include "csfml_render.h"
#include "png_render.h"

#include "utils.h"

#define WINDOW_WIDTH 720
#define WINDOW_HEIGHT 640

void selectDemo(int argc, const char *argv[], Scene *scene) {
    if (argc != 2) {
        printf("Usage: %s DEMO_NAME\n", argv[0]);
        printf("Available demos: spheres, snowman, teapot\n");
        scene_loadSpheresDemo(scene);
    } else if (STREQ(argv[1], "spheres")) {
        scene_loadSpheresDemo(scene);
    } else if (STREQ(argv[1], "snowman")) {
        scene_loadSnowmanDemo(scene);
    } else if (STREQ(argv[1], "teapot")) {
        scene_loadTeapotDemo(scene);
    }
}

int main(int argc, const char *argv[]) {
    Raytracer rt;
    raytracer_init(&rt, WINDOW_WIDTH, WINDOW_HEIGHT);
    selectDemo(argc, argv, &rt.scene);    
#ifdef PNG
    png_render(&rt);
#else
    csfml_render(&rt);
#endif
    raytracer_dealloc(&rt);
    return 0;    
}
