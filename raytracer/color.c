//
//  color.c
//  raytracer
//
//  Created by Krzysztof Gabis on 28.03.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#include "color.h"
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

Color color_make(double r, double g, double b, double a) {
    Color c;
    c.r = r, c.g = g, c.b = b, c.a = a;
    return c;
}

Color color_makeFromRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
    return color_make(r / 255.0, g / 255.0, b / 255.0, a / 255.0);
}

Color color_makeFromRGB(unsigned char r, unsigned char g, unsigned char b) {
    return color_makeFromRGBA(r, g, b, 255);
}

Color color_add(Color a, Color b) {
    return color_make((a.r + b.r) / 2.0, (a.g + b.g) / 2.0,
                      (a.b + b.b) / 2.0, (a.a + b.a) / 2.0);
}

Color color_makeFromRGBhex(unsigned int c) {
    return color_makeFromRGB(c >> 16, c >> 8, c);
}

Color color_makeFromRGBAhex(unsigned int c) {
    return color_makeFromRGBA(c >> 24, c >> 16, c >> 8, c);
}

Color color_mult(Color c, double n) {
    double r = MIN(c.r * n, 1.0);
    double g = MIN(c.g * n, 1.0);
    double b = MIN(c.b * n, 1.0);
    double a = MIN(c.a * n, 1.0);
    return color_make(r, g, b, a);
}