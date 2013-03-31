//
//  color.h
//  raytracer
//
//  Created by Krzysztof Gabis on 26.03.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#ifndef raytracer_color_h
#define raytracer_color_h

#define COLOR_BLACK color_make(0.0f, 0.0f, 0.0f, 1.0f)
#define COLOR_WHITE color_make(1.0f, 1.0f, 1.0f, 1.0f)

#define COLOR_RED   color_makeFromRGBA(226, 13, 81, 255)
#define COLOR_GREEN color_makeFromRGBA(27, 229, 106, 255)
#define COLOR_BLUE  color_makeFromRGBA(79, 135, 247, 255)

//#define COLOR_RED   color_make(1.0f, 0.0f, 0.0f, 1.0f)
//#define COLOR_GREEN color_make(0.0f, 1.0f, 0.0f, 1.0f)
//#define COLOR_BLUE  color_make(0.0f, 0.0f, 1.0f, 1.0f)

typedef struct {
    double r, g, b, a;
} Color;

Color color_make(double r, double g, double b, double a);
Color color_makeFromRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
Color color_add(Color a, Color b);
Color color_mult(Color c, double n);

#endif
