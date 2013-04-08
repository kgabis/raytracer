//
//  color.c
//  raytracer
//
//  Created by Krzysztof Gabis on 28.03.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#include "color.h"
#include "utils.h"

Color color_make(double r, double g, double b, double a) {
    Color c;
    c.r = MIN(r, 1.0);
    c.g = MIN(g, 1.0);
    c.b = MIN(b, 1.0);
    c.a = MIN(a, 1.0);
    return c;
}

Color color_makeFromRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
    return color_make(r / 255.0, g / 255.0, b / 255.0, a / 255.0);
}

Color color_makeFromRGB(unsigned char r, unsigned char g, unsigned char b) {
    return color_makeFromRGBA(r, g, b, 255);
}

Color color_add(Color a, Color b) {
    return color_make(a.r + b.r, a.g + b.g, a.b + b.b, a.a + b.a);
}

Color color_addWeighted(Color a, double weightA, Color b, double weightB) {
    double sum = weightA + weightB;
    Color c;
    c.r = ((a.r * weightA) + (b.r * weightB))/ sum;
    c.g = ((a.g * weightA) + (b.g * weightB))/ sum;
    c.b = ((a.b * weightA) + (b.b * weightB))/ sum;
    c.a = 1.0;
    return c;
}

Color color_makeFromRGBhex(unsigned int c) {
    return color_makeFromRGB(c >> 16, c >> 8, c);
}

Color color_makeFromRGBAhex(unsigned int c) {
    return color_makeFromRGBA(c >> 24, c >> 16, c >> 8, c);
}

Color color_mult(Color c, double n) {
    return color_make(c.r * n, c.g * n, c.b * n, 1.0);
    return c;
}

Color color_blend(Color a, double weightA, Color b) {
    weightA = LIMIT(weightA, 0.0, 1.0);
    double weightB = 1.0 - weightA;
    return color_addWeighted(a, weightA, b, weightB);
}