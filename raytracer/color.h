//
//  color.h
//  raytracer
//
//  Created by Krzysztof Gabis on 26.03.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#ifndef raytracer_color_h
#define raytracer_color_h

#define COLOR_BLACK color_makeFromRGBhex(0x000000)
#define COLOR_WHITE color_makeFromRGBhex(0xFFFFFF)
#define COLOR_RED   color_makeFromRGBhex(0xFF0000)
#define COLOR_GREEN color_makeFromRGBhex(0x00FF00)
#define COLOR_BLUE  color_makeFromRGBhex(0x0000FF)

// http://www.colourlovers.com/palette/433018/Gasoline_Rainbow
#define COLORS1_RED    color_makeFromRGB(189, 42, 51)
#define COLORS1_YELLOW color_makeFromRGB(214, 170, 38)
#define COLORS1_GREEN  color_makeFromRGB(64, 129, 86)
#define COLORS1_GREEN2 color_makeFromRGB(147, 163, 28)
#define COLORS1_BLUE   color_makeFromRGB(48, 55, 79)

// http://www.colourlovers.com/palette/412963/Retro_Rainbow
#define COLORS2_RED    color_makeFromRGBhex(0xC00F12)
#define COLORS2_ORANGE color_makeFromRGBhex(0xD27701)
#define COLORS2_YELLOW  color_makeFromRGBhex(0xDDBA01)
#define COLORS2_GREEN color_makeFromRGBhex(0x007A28)
#define COLORS2_BLUE   color_makeFromRGBhex(0x3B9B95)

typedef struct {
    double r, g, b;
} Color;

Color color_make(double r, double g, double b);
Color color_makeFromRGB(unsigned char r, unsigned char g, unsigned char b);
Color color_makeFromRGBhex(unsigned int c);
Color color_add(Color a, Color b);
Color color_addWeighted(Color a, double weightA, Color b, double weightB);
Color color_mult(Color c, double n);
Color color_blend(Color a, double weightA, Color b);

#endif
