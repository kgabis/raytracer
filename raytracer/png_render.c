//
//  png_main.c
//  raytracer
//
//  Created by Krzysztof Gabis on 13.04.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//
#ifdef PNG

#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include "png_render.h"

#define SET_PIXEL(bmp, x, y, color) (bmp->pixels[((bmp->width)*y)+(x)]=color)
#define GET_PIXEL(bmp, x, y) (bmp->pixels[((bmp->width)*y)+(x)])

typedef struct {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} Pixel;

typedef struct  {
    Pixel *pixels;
    size_t width;
    size_t height;
} Bitmap;

static void draw(void *data, Color color, size_t x, size_t y) {
    Bitmap *picture = (Bitmap*)data;
    Pixel pixel;
    pixel.red = (unsigned char)(color.r * 255.0);
    pixel.green = (unsigned char)(color.g * 255.0);
    pixel.blue = (unsigned char)(color.b * 255.0);
    SET_PIXEL(picture, x, y, pixel);
}

static int png_saveToFile(Bitmap *bitmap, const char *filename) {
    FILE *fp;
    png_structp pngPtr = NULL;
    png_infop infoPtr = NULL;
    png_byte **rowPointers = NULL;
    size_t x, y;    
    int status = -1;
    int pixelSize = 3;
    int depth = 8;
    
    fp = fopen(filename, "wb");
    if(fp == NULL) {
        goto fopen_failed;
    }
    
    pngPtr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (pngPtr == NULL) {
        goto png_create_write_struct_failed;
    }
    
    infoPtr = png_create_info_struct(pngPtr);
    if (infoPtr == NULL) {
        goto png_create_info_struct_failed;
    }
    
    if (setjmp(png_jmpbuf(pngPtr))) {
        goto png_failure;
    }
    
    png_set_IHDR(pngPtr,
                 infoPtr,
                 (png_uint_32)bitmap->width,
                 (png_uint_32)bitmap->height,
                 depth,
                 PNG_COLOR_TYPE_RGB,
                 PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT,
                 PNG_FILTER_TYPE_DEFAULT);
    
    rowPointers = png_malloc(pngPtr, bitmap->height * sizeof(png_byte *));
    for (y = 0; y < bitmap->height; ++y) {
        png_byte *row = png_malloc(pngPtr, sizeof(unsigned char) * bitmap->width * pixelSize);
        rowPointers[y] = row;
        for (x = 0; x < bitmap->width; ++x) {
            Pixel pixel = GET_PIXEL(bitmap, x, y);
            *row++ = pixel.red;
            *row++ = pixel.green;
            *row++ = pixel.blue;
        }
    }
    
    png_init_io(pngPtr, fp);
    png_set_rows(pngPtr, infoPtr, rowPointers);
    png_write_png(pngPtr, infoPtr, PNG_TRANSFORM_IDENTITY, NULL);
    
    status = 0;    
    for (y = 0; y < bitmap->height; y++) {
        png_free(pngPtr, rowPointers[y]);
    }
    png_free(pngPtr, rowPointers);
    
png_failure:
png_create_info_struct_failed:
    png_destroy_write_struct(&pngPtr, &infoPtr);
png_create_write_struct_failed:
    fclose(fp);
fopen_failed:
    return status;
}

void png_render(Raytracer *raytracer) {
    Bitmap renderedImage;
    char filename[256];
    char *dateString;
    filename[0] = '\0';
    time_t rawTime;
    struct tm *timeInfo;
    time(&rawTime);
    timeInfo = localtime(&rawTime);
    renderedImage.width = raytracer->resolutionX;
    renderedImage.height = raytracer->resolutionY;
    renderedImage.pixels = calloc(sizeof(Pixel), renderedImage.width * renderedImage.height);
    dateString = asctime(timeInfo);
    dateString[strlen(dateString) - 1] = '\0';
    sprintf(filename, "%s.png", dateString);
    raytracer_render(raytracer, draw, (void*)&renderedImage);
    int status = png_saveToFile(& renderedImage, filename);
    if (status == 0) {
        printf("Output at: %s\n", filename);
    } else {
        printf("Error!\n");
    }
    
}

#endif
