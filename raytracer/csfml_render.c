//
//  main.c
//  raytracer
//
//  Created by Krzysztof Gabis on 31.03.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#ifndef PNG
#include <SFML/Graphics.h>
#include <SFML/Graphics/Sprite.h>
#include <stdio.h>
#include <string.h>

#include "csfml_render.h"

static void draw(void *data, Color color, size_t x, size_t y) {
    sfImage *screen = (sfImage*)data;
    sfColor sfcolor;
    sfcolor.r = (sfUint8)(color.r * 255.0);
    sfcolor.g = (sfUint8)(color.g * 255.0);
    sfcolor.b = (sfUint8)(color.b * 255.0);
    sfcolor.a = 255;
    sfImage_setPixel(screen, (int)x, (int)y, sfcolor);
}

void csfml_render(Raytracer *raytracer) {
    sfVideoMode mode;
    mode.width = (unsigned int)raytracer->resolutionX;
    mode.height = (unsigned int)raytracer->resolutionY;
    mode.bitsPerPixel = 32;
    sfIntRect bounds;
    bounds.top = 0;
    bounds.left = 0;
    bounds.width = (unsigned int)raytracer->resolutionX;
    bounds.height = (unsigned int)raytracer->resolutionY;
    sfRenderWindow *window;
    sfEvent event;
    sfImage *screen = sfImage_createFromColor(bounds.width, bounds.height, sfWhite);
    sfSprite *sprite = sfSprite_create();
    sfTexture *texture = sfTexture_createFromImage(screen, &bounds);
    sfClock *clock = sfClock_create();
    sfTime time;
    sfSprite_setTexture(sprite, texture, sfFalse);
    window = sfRenderWindow_create(mode, "raytracer", sfClose, NULL);
    if (!window) {
        return;
    }
    while (sfRenderWindow_isOpen(window)) {
        sfClock_restart(clock);
        sfImage_destroy(screen);
        screen = sfImage_createFromColor(bounds.width, bounds.height, sfWhite);
        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
        }
        raytracer_render(raytracer, draw, (void*)screen);
        sfTexture_updateFromImage(texture, screen, 0, 0);
        sfRenderWindow_clear(window, sfWhite);
        sfTexture_bind(texture);
        sfRenderWindow_drawSprite(window, sprite, NULL);
        sfRenderWindow_display(window);
        time = sfClock_getElapsedTime(clock);
        printf("Seconds per frame: %f\n", sfTime_asSeconds(time));
    }
    sfImage_saveToFile(screen, "raytracer_output.png");
    sfImage_destroy(screen);
    sfTexture_destroy(texture);
    sfSprite_destroy(sprite);
    sfRenderWindow_destroy(window);
}
#endif
