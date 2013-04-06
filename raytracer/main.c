//
//  main.c
//  raytracer
//
//  Created by Krzysztof Gabis on 31.03.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#include <SFML/Graphics.h>
#include <SFML/Graphics/Sprite.h>
#include <stdio.h>

#include "raytracer.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

void draw(void *data, Color color, size_t x, size_t y) {
    sfImage *screen = (sfImage*)data;
    sfColor sfcolor;
    sfcolor.r = (sfUint8)(color.r * 255.0);
    sfcolor.g = (sfUint8)(color.g * 255.0);
    sfcolor.b = (sfUint8)(color.b * 255.0);
    sfcolor.a = (sfUint8)(color.a * 255.0);
    sfImage_setPixel(screen, (int)x, (int)y, sfcolor);
}

int main() {
    sfVideoMode mode = {WINDOW_WIDTH, WINDOW_HEIGHT, 32};
    sfIntRect bounds = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
    sfRenderWindow *window;
    sfEvent event;
    sfImage *screen = sfImage_createFromColor(WINDOW_WIDTH, WINDOW_HEIGHT, sfWhite);
    sfSprite *sprite = sfSprite_create();
    sfTexture *texture = sfTexture_createFromImage(screen, &bounds);
    Raytracer rt;
    raytracer_init(&rt, WINDOW_WIDTH, WINDOW_HEIGHT);
    sfClock *clock = sfClock_create();
    sfTime time;
    sfSprite_setTexture(sprite, texture, sfFalse);
    raytracer_loadDemo(&rt);
    window = sfRenderWindow_create(mode, "raytracer", sfClose, NULL);
    if (!window) {
        return 1;
    }
    while (sfRenderWindow_isOpen(window)) {
        sfClock_restart(clock);
        sfImage_destroy(screen);
        screen = sfImage_createFromColor(WINDOW_WIDTH, WINDOW_HEIGHT, sfWhite);
        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
        }
        raytracer_render(&rt, draw, (void*)screen);
        sfTexture_updateFromImage(texture, screen, 0, 0);
        sfRenderWindow_clear(window, sfWhite);
        sfTexture_bind(texture);
        sfRenderWindow_drawSprite(window, sprite, NULL);
        sfRenderWindow_display(window);
        time = sfClock_getElapsedTime(clock);
        printf("Seconds per frame: %f\n", sfTime_asSeconds(time));
        //        sfSleep(sfSeconds(1.0f));
    }
    
    sfImage_destroy(screen);
    sfTexture_destroy(texture);
    sfSprite_destroy(sprite);
    sfRenderWindow_destroy(window);
    raytracer_dealloc(&rt);
    return 0;
}

