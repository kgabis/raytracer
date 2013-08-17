//
//  ray.c
//  raytracer
//
//  Created by Krzysztof Gabis on 01.04.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ray.h"
#include "utils.h"

#define MAX_RECURSION_DEPTH 4
#define MAX_VISIBLE_DISTANCE 600.0

#define EPSILON 0.000001

typedef struct {
    const Surface *surface;
    double distance;
} TracingResult;

typedef struct {
    double diffused;
    double specular;
} ShadingResult;

static Ray ray_addNoise(const Ray *ray, double magnitude);
static Ray ray_reflect(const Ray *ray, const Surface *surface, Vector3 point);
static Color ray_traceRecursive(const Ray *ray, const Scene *scene, size_t depth);
static TracingResult ray_traceOnce(const Ray *ray, const Scene *scene);
static int ray_checkIntersection(const Ray *ray, const Surface *surface, double *distance);
static int ray_checkSphereIntersection(const Ray *ray, const Sphere *sphere, double *distance);
static int ray_checkTriangleIntersection(const Ray *ray, const Triangle *triangle, double *distance);

static ShadingResult ray_shadeAtPoint(const Ray *ray, const Scene *scene, const Surface *surface, Vector3 point);
static Color getHighlightedColor(Color color, ShadingResult highlight, double ambientCoef);

Ray ray_make(Vector3 origin, Vector3 direction) {
    Ray r = { .origin = origin, .direction = direction};
    return r;
}

Ray ray_makeForPixel(const Camera *c, size_t x, size_t y) {
    double dy = 1.0;
    double dx = 1.0;
    double py = (- c->height / 2.0) + dy * ((double)y + 0.5);
    double px = (- c->width / 2.0) + dx * ((double)x + 0.5);
    Vector3 p = vec3_add3(c->planeCenter,
                          vec3_mult(c->planeDirectionX, px),
                          vec3_mult(c->planeDirectionY, py));
    Vector3 u_r = vec3_unit(vec3_sub(p, c->position));
    return ray_make(c->position, u_r);
}

Color ray_trace(const Ray *ray, const Scene *scene) {
    return ray_traceRecursive(ray, scene, MAX_RECURSION_DEPTH);
}

static Color ray_traceRecursive(const Ray *ray, const Scene *scene, size_t depth) {
    TracingResult closestHit = ray_traceOnce(ray, scene);
    Color resultColor;
    if (closestHit.surface == NULL) {
        return scene->backgroundColor;
    }
    resultColor = closestHit.surface->material.color;
    Vector3 collisionPoint = vec3_add(vec3_mult(ray->direction, closestHit.distance), ray->origin);
    Material material = closestHit.surface->material;
    if (material.reflectivity > 0.0 && depth > 0) {
        Ray reflectedRay = ray_reflect(ray, closestHit.surface, collisionPoint);
        if (material.reflectionNoise > 0) {
            reflectedRay = ray_addNoise(&reflectedRay, material.reflectionNoise);
        }
        Color reflectionColor = ray_traceRecursive(&reflectedRay, scene, depth - 1);
        resultColor = color_blend(reflectionColor, material.reflectivity, resultColor);
    }
    ShadingResult shadingResult = ray_shadeAtPoint(ray, scene, closestHit.surface, collisionPoint);    
    resultColor = getHighlightedColor(resultColor, shadingResult, scene->ambientCoefficient);
    resultColor = color_mult(resultColor, (MAX_VISIBLE_DISTANCE - closestHit.distance) / MAX_VISIBLE_DISTANCE);
    return resultColor;
}

static TracingResult ray_traceOnce(const Ray *ray, const Scene *scene) {
    TracingResult closestHit = { .surface = NULL, .distance = 1.0 / 0.0 };
    double distance = 1.0 / 0.0f;
    int hit = 0;
    for (size_t i = 0; i < scene->surfaces.count; i++) {
        Surface *surface = ARRAY_GET(&scene->surfaces, i);
        hit = ray_checkIntersection(ray, surface, &distance);
        if (hit && distance < closestHit.distance && distance > EPSILON) {
            closestHit.distance = distance;
            closestHit.surface = surface;
        }
    }
    return closestHit;
}

static ShadingResult ray_shadeAtPoint(const Ray *ray, const Scene *scene, const Surface *surface, Vector3 point) {
    size_t i;
    Light *light;
    ShadingResult shadingResult;
    shadingResult.diffused = 0.0;
    shadingResult.specular = 0.0;
    double lightDistance;
    Vector3 lightDirection;
    Ray newRay;
    newRay.origin = point;
    TracingResult shadowTracingResult;
    for (i = 0; i < scene->lights.count; i++) {
        light = ARRAY_GET(&scene->lights, i);
        lightDirection = light_getDirection(light, point);
        newRay.direction = vec3_negate(lightDirection);
        lightDistance = vec3_length(vec3_sub(light->position, point));
        shadowTracingResult = ray_traceOnce(&newRay, scene);
        if (shadowTracingResult.surface == NULL || shadowTracingResult.distance > lightDistance) {
            Vector3 normal = surface_getNormalAtPoint(surface, point);
            shadingResult.diffused += light_getDiffusedHighlight(light, lightDirection, normal);
            shadingResult.specular += light_getSpecularHighlight(light, lightDirection,
                                                                 normal, ray->direction,
                                                                 surface->material.specularity);
        }
    }
    return shadingResult;
}

static Ray ray_addNoise(const Ray *ray, double epsilon) {
    double r = (((double)rand()/RAND_MAX) * 2 * epsilon) - epsilon;
    Ray newRay = *ray;
    newRay.direction.x += r;
    r = (((double)rand()/RAND_MAX) * 2 * epsilon) - epsilon;
    newRay.direction.y += r;
    r = (((double)rand()/RAND_MAX) * 2 * epsilon) - epsilon;
    newRay.direction.z += r;
    newRay.direction = vec3_unit(ray->direction);
    return newRay;
}

static Ray ray_reflect(const Ray *ray, const Surface *surface, Vector3 point) {
    Vector3 surfaceNormal = surface_getNormalAtPoint(surface, point);
    double c1 = - vec3_dot(surfaceNormal, ray->direction);
    Vector3 RI = vec3_add(ray->direction, vec3_mult(surfaceNormal, 2 * c1));
    return ray_make(point, RI);
}

static int ray_checkIntersection(const Ray *ray, const Surface *surface, double *distance) {
    switch (surface->type) {
        case GTSphere:
            return ray_checkSphereIntersection(ray, &surface->geometry.sphere, distance);
            break;
        case GTTriangle:
            return ray_checkTriangleIntersection(ray, &surface->geometry.triangle, distance);
            break;
        default:
            break;
    }
    return 0;
}

// http://www.cs.unc.edu/~rademach/xroads-RT/RTarticle.html
//static TracingResult ray_checkSphereIntersection_1(const Ray *ray, const Sphere *s) {
//    TracingResult result;
//    result.hit = 0;
//    Vector3 EO = vec3_sub(s->center, ray->origin);
//    double v = vec3_dot(EO, ray->direction);
//    if (v < 0) {
//        return result;
//    }
//    double r = s->radius;
//    double disc = SQUARE(r) - (vec3_dot(EO, EO) - SQUARE(v));
//    if (disc < 0.0) {
//        return result;
//    }
//    double d = sqrt(disc);
//    result.distance = MIN(v - d, v + d);
//    result.hit = 1;
//    return result;
//}

// http://stackoverflow.com/questions/14074643/why-does-raytracer-render-spheres-as-ovals
static int ray_checkSphereIntersection(const Ray *ray, const Sphere *sphere, double *distance) {
    double a = vec3_dot(ray->direction, ray->direction);
    Vector3 ro_sc = vec3_sub(ray->origin, sphere->center);
    double b = 2.0 * vec3_dot(ray->direction, ro_sc);
    double c = vec3_dot(ro_sc, ro_sc) - SQUARE(sphere->radius);
    double disc = SQUARE(b) - 4 * a * c;
    if (disc < 0) {
        return 0;
    }    
    double distSqrt = sqrt(disc);
    double q = b < 0.0 ? (-b - distSqrt) / 2.0 : (-b + distSqrt) / 2.0;
    double t0 = q / a;
    double t1 = c / q;
    if (t0 > t1) {
        double swap = t0;
        t0 = t1;
        t1 = swap;
    }
    if (t1 < 0) {
        return 0;
    } else if(t0 < 0) {
        *distance = t1;
    } else {
        *distance = t0;
    }
    return 1;
}

// http://www.cs.virginia.edu/~gfx/Courses/2003/ImageSynthesis/papers/Acceleration/Fast%20MinimumStorage%20RayTriangle%20Intersection.pdf
static int ray_checkTriangleIntersection(const Ray *ray, const Triangle *t, double *distance) {
    Vector3 pvec, tvec, qvec;
    VEC3_CROSS(pvec, ray->direction, t->edges[0]);
    double det = VEC3_DOT(t->edges[1], pvec);
#define EPSILON 0.000001
    if (det < EPSILON) {
        return 0;
    }
#undef EPSILON
    VEC3_SUB(tvec, ray->origin, t->a);
    double u = VEC3_DOT(tvec, pvec);
    if (u < 0.0 || u > det) {
        return 0;
    }
    VEC3_CROSS(qvec, tvec, t->edges[1]);
    double v = VEC3_DOT(ray->direction, qvec);
    if (v < 0.0 || u + v > det) {
        return 0;
    }
    double d = VEC3_DOT(t->edges[0], qvec);
    double inv_det = 1.0 / det;
    d *= inv_det;
//    u *= inv_det; // may be useful in the future to get texture coordinates
//    v *= inv_det;
    *distance = d;
    return 1;
}

Color getHighlightedColor(Color color, ShadingResult highlight, double ambientCoef) {
    double diffusedCoef = 1.0 - ambientCoef;
    color = color_mult(color, ambientCoef + highlight.diffused * diffusedCoef);
    color = color_add(color, color_mult(COLOR_WHITE, highlight.specular));
    return color;
}
