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
#define SQUARE(x) ((x)*(x))
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define MAX_VISIBLE_DISTANCE 1000.0

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
TracingResult ray_checkSphereIntersection_1(Ray ray, Sphere sphere);
TracingResult ray_checkSphereIntersection_2(Ray ray, Sphere sphere);
TracingResult ray_checkTriangleIntersection(Ray ray, Triangle triangle);

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
    
    camera_init(&rt->camera, 250.0, resolutionX, resolutionY);
    
    rt->backgroundColor = COLOR_WHITE;
    return rt;
}

void raytracer_loadDemo(Raytracer *rt) {
    Object spheres[3];
    Object triangles[10];
    Vector3 vs[8];
    spheres[0] = object_initSphere(vec3_make(0, 45, 100), 25,
                                       material_make(COLOR_RED, 0));
    spheres[1] = object_initSphere(vec3_make(15, 10, 100), 30,
                                       material_make(COLOR_GREEN, 0));
    spheres[2] = object_initSphere(vec3_make(-15, 10, 100), 35,
                                       material_make(COLOR_BLUE, 0.0));
    Material sideWallMaterial1 = material_make(COLORS1_BLUE, 0.0);
    Material sideWallMaterial2 = material_make(COLORS1_GREEN, 0.0);
    Material ceilingMaterial = material_make(COLORS2_RED, 0.0);
    Material floorMaterial = material_make(COLORS2_RED, 0.0);
    vs[0] = vec3_make(-75, -40, 50);
    vs[1] = vec3_make(-75, -40, 150);
    vs[2] = vec3_make(75, -40, 50);
    vs[3] = vec3_make(75, -40, 150);
    vs[4] = vec3_make(-75, 110, 50);
    vs[5] = vec3_make(-75, 110, 150);
    vs[6] = vec3_make(75, 110, 50);
    vs[7] = vec3_make(75, 110, 150);
    triangles[0] = object_initTriangle(vs[0], vs[1], vs[2], floorMaterial);
    triangles[1] = object_initTriangle(vs[1], vs[3], vs[2], floorMaterial);
    triangles[2] = object_initTriangle(vs[4], vs[1], vs[0], sideWallMaterial1);
    triangles[3] = object_initTriangle(vs[4], vs[5], vs[1], sideWallMaterial1);
    triangles[4] = object_initTriangle(vs[6], vs[3], vs[2], sideWallMaterial1);
    triangles[5] = object_initTriangle(vs[6], vs[7], vs[3], sideWallMaterial1);
    triangles[6] = object_initTriangle(vs[4], vs[5], vs[6], ceilingMaterial);
    triangles[7] = object_initTriangle(vs[5], vs[7], vs[6], ceilingMaterial);
    triangles[8] = object_initTriangle(vs[5], vs[7], vs[3], sideWallMaterial2);
    triangles[9] = object_initTriangle(vs[5], vs[1], vs[3], sideWallMaterial2);
    raytracer_addObjectRange(rt, spheres, 3);
    raytracer_addObjectRange(rt, triangles, 10);
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

void raytracer_addObjectRange(Raytracer *rt, Object *objects, size_t len) {
    for (size_t i = 0; i < len; i++) {
        raytracer_addObject(rt, objects[i]);
    }
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
    free(rt->objects.objects);
    free(rt);
}

void camera_init(Camera *cam, double focalLength, double width, double height) {
    cam->position = vec3_make(0, 10, 0);
    cam->direction = vec3_make(0, 0, 1);
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
        closestHit.color = color_mult(closestHit.color, (MAX_VISIBLE_DISTANCE - closestHit.distance) / MAX_VISIBLE_DISTANCE);
    }
    return closestHit;
}

TracingResult ray_checkIntersection(Ray ray, Object *object) {
    TracingResult result;
    result.hit = 0;
    switch (object->type) {
        case GTSphere:
            result = ray_checkSphereIntersection_2(ray, object->geometry.sphere);
            result.object = object;
            result.color = object->material.color;
            break;
        case GTTriangle:
            result = ray_checkTriangleIntersection(ray, object->geometry.triangle);
            result.object = object;
            result.color = object->material.color;
            break;
        default:
            break;
    }
    return result;
}

// http://www.cs.unc.edu/~rademach/xroads-RT/RTarticle.html
TracingResult ray_checkSphereIntersection_1(Ray ray, Sphere sphere) {
    TracingResult result;
    result.hit = 0;
    Vector3 EO = vec3_sub(sphere.center, ray.origin);
    double v = vec3_dot(EO, ray.direction);
    double r = sphere.radius;
    double disc = SQUARE(r) - (vec3_dot(EO, EO) - SQUARE(v));
    if (disc < 0.0) {
        return result;
    }
    double d = sqrt(disc);
    result.distance = MIN(v - d, v + d);
    result.hit = 1;
    return result;
}

// http://www.cs.princeton.edu/courses/archive/fall00/cs426/lectures/raycast/sld013.htm
TracingResult ray_checkSphereIntersection_2(Ray ray, Sphere sphere) {
    TracingResult result;
    result.hit = 0;
    Vector3 O = sphere.center;
    Vector3 P_0 = ray.origin;
    Vector3 V = ray.direction;
    Vector3 L;
    VEC3_SUB(L, O, P_0);
    double t_ca = VEC3_DOT(L, V);
    double d2 = VEC3_DOT(L, L) - SQUARE(t_ca);
    double r2 = SQUARE(sphere.radius);
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

// http://www.cs.virginia.edu/~gfx/Courses/2003/ImageSynthesis/papers/Acceleration/Fast%20MinimumStorage%20RayTriangle%20Intersection.pdf
TracingResult ray_checkTriangleIntersection(Ray ray, Triangle t) {
    TracingResult result;
    result.hit = 0;
    Vector3 pvec, tvec, qvec;
    VEC3_CROSS(pvec, ray.direction, t.edges[1]);
    double det = VEC3_DOT(t.edges[0], pvec);
#define EPSILON 0.000001
    // Version without culling
    if (det > -EPSILON && det < EPSILON) {
        return result;
    }
    double inv_det = 1.0 / det;
    VEC3_SUB(tvec, ray.origin, t.a);
    double u = VEC3_DOT(tvec, pvec) * inv_det;
    if (u < 0.0 || u  > 1.0) {
        return result;
    }
    VEC3_CROSS(qvec, tvec, t.edges[0]);
    double v = VEC3_DOT(ray.direction, qvec) * inv_det;
    if (v < 0.0 || u + v > 1.0) {
        return result;
    }
    double d = VEC3_DOT(t.edges[1], qvec) * inv_det;
////////////////////////////////////////////////
// Version with culling
//    if (det < EPSILON) {
//        return result;
//    }
//    VEC3_SUB(tvec, ray.origin, t.a);
//    double u = VEC3_DOT(tvec, pvec);
//    if (u < 0.0 || u > det) {
//        return result;
//    }
//    VEC3_CROSS(qvec, tvec, edge1);
//    double v = VEC3_DOT(ray.direction, qvec);
//    if (v < 0.0 || u + v > det) {
//        return result;
//    }
//    double d = VEC3_DOT(edge2, qvec);
//    double inv_det = 1.0 / det;
//    d *= inv_det;
//    u *= inv_det;
//    v *= inv_det;
////////////////////////////////////////////////
#undef EPSILON
    
    result.distance = d;
    result.hit = 1;
    return result;
}


