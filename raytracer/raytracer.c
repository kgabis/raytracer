//
//  raytracer.c
//  raytracer
//
//  Created by Krzysztof Gabis on 29.03.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "raytracer.h"

#define INIT_CAPACITY 256
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define MAX_DISTANCE 500.0

typedef struct {
    Vector3 origin;
    Vector3 direction;
} Ray;

typedef struct {
    int hit;
    Color color;
    Object *object;
    double distance;
} TracingResult;

void camera_init(Camera *c, double focalLength, double width, double height);
void camera_setup(Camera *c);
Ray getRayForPixel(Camera *c, size_t x, size_t y);
TracingResult ray_trace(Ray ray, Raytracer *rt);
TracingResult ray_checkIntersection(Ray ray, Object *object);
TracingResult ray_checkSphereIntersection(Ray ray, Sphere *sphere);

Raytracer* raytracer_init(size_t resolutionX, size_t resolutionY) {
    //    printf("Object: %lu\n", sizeof(Object));
    //    printf("Sphere: %lu\n", sizeof(Sphere));
    Raytracer *rt = malloc(sizeof(Raytracer));
    if (rt == NULL) {
        return NULL;
    }
    rt->objects.objects = malloc(sizeof(Object) * INIT_CAPACITY);
    if (rt->objects.objects == NULL) {
        free(rt);
        return NULL;
    }
    rt->objects.capacity = INIT_CAPACITY;
    rt->objects.len = 0;
    
    rt->resolutionX = resolutionX;
    rt->resolutionY = resolutionY;
    
    camera_init(&rt->camera, 500.0, resolutionX, resolutionY);
    
    rt->backgroundColor = COLOR_WHITE;
    return rt;
}

void raytracer_loadDemo(Raytracer *rt) {
    raytracer_addObject(rt, object_initSphere(vec3_make(0.0, 15.0, 150.0), 15.0, COLOR_RED));
    raytracer_addObject(rt, object_initSphere(vec3_make(10.0, -10.0, 150.0), 20.0, COLOR_GREEN));
    raytracer_addObject(rt, object_initSphere(vec3_make(-10.0, -10.0, 150.0), 25.0, COLOR_BLUE));
}

void raytracer_addObject(Raytracer *rt, Object object) {
    void *reallocPtr = NULL;
    size_t newCapacity;
    if (rt->objects.len >= rt->objects.capacity) {
        newCapacity = rt->objects.capacity * 2;
        reallocPtr = realloc(rt->objects.objects, sizeof(Object) * newCapacity);
        if (reallocPtr == NULL) {
            return;
        }
        rt->objects.objects = reallocPtr;
        rt->objects.capacity = newCapacity;
    }
    rt->objects.objects[rt->objects.len] = object;
    rt->objects.len++;
}

void raytracer_render(Raytracer *rt, DrawFunction draw, void *data) {
    camera_setup(&rt->camera);
    //    rt->objects.objects[2].geometry.sphere.center.z += 1.0;
    for (size_t x = 0; x < rt->resolutionX; x++) {
        for (size_t y = 0; y < rt->resolutionY; y++) {
            Ray ray = getRayForPixel(&rt->camera, x, y);
            TracingResult result = ray_trace(ray, rt);
            Color color;
            if (!result.hit) {
                color = rt->backgroundColor;
            } else {
                color = result.color;
            }
            draw(data, color, x, y);
        }
    }
}

void raytracer_dealloc(Raytracer *rt) {
    free(rt);
    free(rt->objects.objects);
}

void camera_init(Camera *cam, double focalLength, double width, double height) {
    cam->position = VECTOR3_ZERO;
    cam->direction = vec3_make(0.0, 0.0, 1.0);
    cam->focalLength = focalLength;
    cam->width = width;
    cam->height = height;
}

// See: http://people.cs.clemson.edu/~dhouse/courses/405/notes/raycamera.pdf
void camera_setup(Camera *c) {
    Vector3 x_c = c->position;
    Vector3 u_c = c->direction;
    double z_p = c->focalLength;
    Vector3 v_up = vec3_make(0.0, -1.0, 0.0);
    Vector3 c_0 = vec3_add(x_c, vec3_mult(u_c, z_p));
    Vector3 u_x = vec3_unit(vec3_cross(u_c, v_up));
    Vector3 u_y = vec3_cross(vec3_mult(u_c, -1.0), u_x);
    c->planeCenter = c_0;
    c->planeDirectionX = u_x;
    c->planeDirectionY = u_y;
    //    Vector3 u_z = vec3_mult(u_c, -1.0); // Normal to the view plane
}

Ray getRayForPixel(Camera *c, size_t x, size_t y) {
    Ray ray;
    Vector3 c_0 = c->planeCenter;
    Vector3 u_x = c->planeDirectionX;
    Vector3 u_y = c->planeDirectionY;
    //    Vector3 u_z = vec3_mult(u_c, -1.0); // Normal to the view plane
    double dy = 1.0;
    double dx = 1.0;
    double py = (- c->height / 2.0) + dy * (y + 0.5);
    double px = (- c->width / 2.0) + dx * (x + 0.5);
    Vector3 p = vec3_add3(c_0, vec3_mult(u_x, px), vec3_mult(u_y, py));
    Vector3 u_r = vec3_unit(vec3_sub(p, c->position));
    ray.origin = c->position;
    ray.direction = u_r;
    return ray;
}

TracingResult ray_trace(Ray ray, Raytracer *rt) {
    TracingResult closestHit;
    TracingResult currentHit;
    closestHit.hit = 0;
    closestHit.distance = 1.0/0.0; // infinity
    for (size_t i = 0; i < rt->objects.len; i++) {
        currentHit = ray_checkIntersection(ray, &rt->objects.objects[i]);
        if (currentHit.hit && currentHit.distance < closestHit.distance) {
            closestHit = currentHit;
        }
    }
    if (closestHit.hit) {
        closestHit.color = color_mult(closestHit.color, (MAX_DISTANCE - closestHit.distance) / MAX_DISTANCE);
    }
    return closestHit;
}

TracingResult ray_checkIntersection(Ray ray, Object *object) {
    TracingResult result;
    result.hit = 0;
    switch (object->type) {
        case GTSphere:
            result = ray_checkSphereIntersection(ray, &object->geometry.sphere);
            result.object = object;
            result.color = object->material.color;
            break;
        case GTTriangle:
            result.hit = 0;
            break;
        default:
            break;
    }
    return result;
}
// http://www.cs.princeton.edu/courses/archive/fall00/cs426/lectures/raycast/sld013.htm
TracingResult ray_checkSphereIntersection(Ray ray, Sphere *sphere) {
    TracingResult result;
    result.hit = 0;
    Vector3 O = sphere->center;
    Vector3 P_0 = ray.origin;
    Vector3 V = ray.direction;
    Vector3 L = vec3_sub(O, P_0);
    double t_ca = vec3_dot(L, V);
    double d2 = vec3_dot(L, L) - (t_ca * t_ca);
    double r2 = sphere->radius * sphere->radius;
    if (d2 > r2) {
        return result;
    }
    double t_hc = sqrt(r2 - d2);
    double t1 = t_ca - t_hc;
    double t2 = t_ca + t_hc;
    result.distance = MIN(t1, t2);
    result.hit = 1;
    return result;
}