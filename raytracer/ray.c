//
//  ray.c
//  raytracer
//
//  Created by Krzysztof Gabis on 01.04.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#include <stdio.h>
#include <math.h>

#include "ray.h"

#define MAX_RECURSION_DEPTH 1
#define SQUARE(x) ((x)*(x))
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define MAX_VISIBLE_DISTANCE 300.0

typedef struct {
    int hit;
    Color color;
    const Object *object;
    double distance;
} TracingResult;

static double getShade(Vector3 normal, Light ambientLight);
static TracingResult ray_traceReflection(Ray ray, Scene *scene, const Object *toExclude, size_t depth);
static TracingResult ray_checkIntersection(Ray ray, const Object *object);
static TracingResult ray_checkSphereIntersection_1(Ray ray, const Sphere *sphere);
static TracingResult ray_checkSphereIntersection_2(Ray ray, const Sphere *sphere);
static TracingResult ray_checkTriangleIntersectionWithCulling(Ray ray, const Triangle *triangle);
static TracingResult ray_checkTriangleIntersectionNoCulling(Ray ray, const Triangle *triangle);

Ray ray_make(Vector3 origin, Vector3 direction) {
    Ray r;
    r.origin = origin, r.direction = direction;
    return r;
}

Ray ray_makeForPixel(const Camera *c, size_t x, size_t y) {
    double dy = 1.0;
    double dx = 1.0;
    double py = (- c->height / 2.0) + dy * (y + 0.5);
    double px = (- c->width / 2.0) + dx * (x + 0.5);
    Vector3 p = vec3_add3(c->planeCenter,
                          vec3_mult(c->planeDirectionX, px),
                          vec3_mult(c->planeDirectionY, py));
    Vector3 u_r = vec3_unit(vec3_sub(p, c->position));
    return ray_make(c->position, u_r);
}

Color ray_trace(Ray ray, const Scene *scene) {
    TracingResult closestHit;
    TracingResult currentHit;
    closestHit.hit = 0;
    closestHit.color = scene->backgroundColor;
    closestHit.distance = 1.0/0.0; // infinity
    for (size_t i = 0; i < scene->objects.count; i++) {
        Object *object = array_get(&scene->objects, i);
        currentHit = ray_checkIntersection(ray, object);
        if (currentHit.hit && currentHit.distance < closestHit.distance) {
            closestHit = currentHit;
        }
    }
    if (!closestHit.hit) {
        return closestHit.color;
    }
    Vector3 collisionPoint = vec3_add(vec3_mult(ray.direction, closestHit.distance), ray.origin);
    double shade = light_getAmbientShade(&scene->ambientLight, object_getNormalAtPoint(closestHit.object, collisionPoint));
    double diffuseCoefficient = 1.0 - scene->ambientCoefficient;
    closestHit.color = color_mult(closestHit.color, scene->ambientCoefficient + diffuseCoefficient * shade);
    closestHit.color = color_mult(closestHit.color, (MAX_VISIBLE_DISTANCE - closestHit.distance) / MAX_VISIBLE_DISTANCE);
    return closestHit.color;
}

static TracingResult ray_traceReflection(Ray ray, Scene *scene, const Object *toExclude, size_t depth) {
    TracingResult closestHit;
    TracingResult currentHit;
    closestHit.hit = 0;
    closestHit.color = scene->backgroundColor;
    closestHit.distance = 1.0/0.0; // infinity
    for (size_t i = 0; i < scene->objects.count; i++) {
        Object *object = array_get(&scene->objects, i);
        if (object == toExclude) {
            continue;
        }
        currentHit = ray_checkIntersection(ray, object);
        if (currentHit.hit && currentHit.distance < closestHit.distance) {
            closestHit = currentHit;
        }
    }
    if (!closestHit.hit) {
        return closestHit;
    }
    closestHit.color = color_mult(closestHit.color, (MAX_VISIBLE_DISTANCE - closestHit.distance) / MAX_VISIBLE_DISTANCE);
    double reflectivity = closestHit.object->material.reflectivity;
    if (reflectivity <= 0.0 || depth == 0) {
        return closestHit;
    }
    Vector3 collisionPoint = vec3_add(vec3_mult(ray.direction, closestHit.distance), ray.origin);
//  r =dir - 2(dir . m)m
    Vector3 m = object_getNormalAtPoint(closestHit.object, collisionPoint);
    double dirDm = vec3_dot(ray.direction, m);
    Vector3 tempVector = vec3_mult(m, 2 * dirDm);
    Vector3 reflectionDirection = vec3_sub(tempVector, ray.direction);
    Ray reflectionRay = ray_make(collisionPoint, reflectionDirection);
    TracingResult reflection = ray_traceReflection(reflectionRay, scene, closestHit.object, depth - 1);
    closestHit.color = color_addWeighted(closestHit.color, 1.0 - reflectivity, reflection.color, reflectivity);
    return closestHit;
}

static TracingResult ray_checkIntersection(Ray ray, const Object *object) {
    TracingResult result;
    result.hit = 0;
    switch (object->type) {
        case GTSphere:
            result = ray_checkSphereIntersection_2(ray, &object->geometry.sphere);
            result.object = object;
            result.color = object->material.color;
            break;
        case GTTriangle:
            result = ray_checkTriangleIntersectionWithCulling(ray, &object->geometry.triangle);
            result.object = object;
            result.color = object->material.color;
            break;
        default:
            break;
    }
    return result;
}

// http://www.cs.unc.edu/~rademach/xroads-RT/RTarticle.html
static TracingResult ray_checkSphereIntersection_1(Ray ray, const Sphere *s) {
    TracingResult result;
    result.hit = 0;
    Vector3 EO = vec3_sub(s->center, ray.origin);
    double v = vec3_dot(EO, ray.direction);
    double r = s->radius;
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
static TracingResult ray_checkSphereIntersection_2(Ray ray, const Sphere *s) {
    TracingResult result;
    result.hit = 0;
    Vector3 O = s->center;
    Vector3 P_0 = ray.origin;
    Vector3 V = ray.direction;
    Vector3 L;
    VEC3_SUB(L, O, P_0);
    double t_ca = VEC3_DOT(L, V);
    double d2 = VEC3_DOT(L, L) - SQUARE(t_ca);
    double r2 = SQUARE(s->radius);
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
static TracingResult ray_checkTriangleIntersectionWithCulling(Ray ray, const Triangle *t) {
    TracingResult result;
    result.hit = 0;
    Vector3 pvec, tvec, qvec;
    VEC3_CROSS(pvec, ray.direction, t->edges[0]);
    double det = VEC3_DOT(t->edges[1], pvec);
#define EPSILON 0.000001
    if (det < EPSILON) {
        return result;
    }
#undef EPSILON
    VEC3_SUB(tvec, ray.origin, t->a);
    double u = VEC3_DOT(tvec, pvec);
    if (u < 0.0 || u > det) {
        return result;
    }
    VEC3_CROSS(qvec, tvec, t->edges[1]);
    double v = VEC3_DOT(ray.direction, qvec);
    if (v < 0.0 || u + v > det) {
        return result;
    }
    double d = VEC3_DOT(t->edges[0], qvec);
    double inv_det = 1.0 / det;
    d *= inv_det;
    u *= inv_det;
    v *= inv_det;    
    result.distance = d;
    result.hit = 1;
    return result;
}

static TracingResult ray_checkTriangleIntersectionNoCulling(Ray ray, const Triangle *t) {
    TracingResult result;
    result.hit = 0;
    Vector3 pvec, tvec, qvec;
    VEC3_CROSS(pvec, ray.direction, t->edges[1]);
    double det = VEC3_DOT(t->edges[0], pvec);
#define EPSILON 0.000001
    if (det > -EPSILON && det < EPSILON) {
        return result;
    }
#undef EPSILON
    double inv_det = 1.0 / det;
    VEC3_SUB(tvec, ray.origin, t->a);
    double u = VEC3_DOT(tvec, pvec) * inv_det;
    if (u < 0.0 || u  > 1.0) {
        return result;
    }
    VEC3_CROSS(qvec, tvec, t->edges[0]);
    double v = VEC3_DOT(ray.direction, qvec) * inv_det;
    if (v < 0.0 || u + v > 1.0) {
        return result;
    }
    double d = VEC3_DOT(t->edges[1], qvec) * inv_det;
    result.distance = d;
    result.hit = 1;
    return result;
}


