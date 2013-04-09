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

typedef struct {
    int hit;
    Color color;
    const Object *object;
    double distance;
} TracingResult;

typedef struct {
    double diffused;
    double specular;
} ShadingResult;

static Ray ray_addNoise(Ray ray, double epsilon);
static Ray ray_reflect(Ray ray, const Object *object, Vector3 point);
static Color ray_traceRecursive(Ray ray, const Scene *scene, size_t depth);
static TracingResult ray_traceOnce(Ray ray, const Scene *scene);
static TracingResult ray_checkIntersection(Ray ray, const Object *object);
//static TracingResult ray_checkSphereIntersection_1(Ray ray, const Sphere *sphere);
static TracingResult ray_checkSphereIntersection_2(Ray ray, const Sphere *sphere);
static TracingResult ray_checkTriangleIntersectionWithCulling(Ray ray, const Triangle *triangle);
//static TracingResult ray_checkTriangleIntersectionNoCulling(Ray ray, const Triangle *triangle);

static ShadingResult ray_shadeAtPoint(Ray ray, TracingResult *result, const Scene *scene, Vector3 point);
static Color getHighlightedColor(Color color, ShadingResult highlight, double ambientCoef);

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
    return ray_traceRecursive(ray, scene, MAX_RECURSION_DEPTH);
}

static Color ray_traceRecursive(Ray ray, const Scene *scene, size_t depth) {
    TracingResult closestHit = ray_traceOnce(ray, scene);
    if (!closestHit.hit) {
        return closestHit.color;
    }
    Vector3 collisionPoint = vec3_add(vec3_mult(ray.direction, closestHit.distance), ray.origin);
    ShadingResult shadingResult = ray_shadeAtPoint(ray, &closestHit, scene, collisionPoint);

    closestHit.color = getHighlightedColor(closestHit.color, shadingResult, scene->ambientCoefficient);

    double reflectivity = closestHit.object->material.reflectivity;
    if (reflectivity > 0.0 && depth > 0) {
        Ray reflectedRay = ray_reflect(ray, closestHit.object, collisionPoint);
        reflectedRay = ray_addNoise(reflectedRay, 0.02);
        Color reflectionColor = ray_traceRecursive(reflectedRay, scene, depth - 1);
        closestHit.color = color_addWeighted(closestHit.color, 1.0 - reflectivity, reflectionColor, reflectivity);
    }
    closestHit.color = color_mult(closestHit.color, (MAX_VISIBLE_DISTANCE - closestHit.distance) / MAX_VISIBLE_DISTANCE);

    return closestHit.color;
}

static TracingResult ray_traceOnce(Ray ray, const Scene *scene) {
    TracingResult closestHit;
    TracingResult currentHit;
    closestHit.hit = 0;
    closestHit.color = scene->backgroundColor;
    closestHit.distance = 1.0 / 0.0; // infinity
    for (size_t i = 0; i < scene->objects.count; i++) {
        Object *object = ARRAY_GET(&scene->objects, i);
        currentHit = ray_checkIntersection(ray, object);
        if (currentHit.hit && currentHit.distance < closestHit.distance && currentHit.distance >= 0) {
            closestHit = currentHit;
        }
    }
    return closestHit;
}

static ShadingResult ray_shadeAtPoint(Ray ray, TracingResult *tracingResult, const Scene *scene, Vector3 point) {
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
        shadowTracingResult = ray_traceOnce(newRay, scene);
        if (!shadowTracingResult.hit || shadowTracingResult.distance > lightDistance) {
            Vector3 normal = object_getNormalAtPoint(tracingResult->object, point);
            shadingResult.diffused += light_getDiffusedHighlight(light, lightDirection, normal);
            shadingResult.specular += light_getSpecularHighlight(light, lightDirection,
                                                                 normal, ray.direction,
                                                                 tracingResult->object->material.specularity);
        }
    }
    return shadingResult;
}

static Ray ray_addNoise(Ray ray, double epsilon) {
    double r = (((double)rand()/RAND_MAX) * 2 * epsilon) - epsilon;
    ray.direction.x += r;
    r = (((double)rand()/RAND_MAX) * 2 * epsilon) - epsilon;
    ray.direction.y += r;
    r = (((double)rand()/RAND_MAX) * 2 * epsilon) - epsilon;
    ray.direction.z += r;
    ray.direction = vec3_unit(ray.direction);
    return ray;
}

static Ray ray_reflect(Ray ray, const Object *object, Vector3 point) {
    Vector3 N = object_getNormalAtPoint(object, point);
    double c1 = - vec3_dot(N, ray.direction);
    Vector3 RI = vec3_add(ray.direction, vec3_mult(N, 2 * c1));
    return ray_make(point, RI);
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
//static TracingResult ray_checkSphereIntersection_1(Ray ray, const Sphere *s) {
//    TracingResult result;
//    result.hit = 0;
//    Vector3 EO = vec3_sub(s->center, ray.origin);
//    double v = vec3_dot(EO, ray.direction);
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
    if (t_ca < 0) {
        return result;
    }
    double d2 = VEC3_DOT(L, L) - SQUARE(t_ca);
    double r2 = SQUARE(s->radius);
    if (d2 > r2) {
        return result;
    }
    double t_hc = sqrt(r2 - d2);
    double t1 = fabs(t_ca - t_hc);
    double t2 = fabs(t_ca + t_hc);
    result.distance = MIN(t1, t2);
    result.hit = 1;
    return result;
}

// http://www.cs.virginia.edu/~gfx/Courses/2003/ImageSynthesis/papers/Acceleration/Fast%20MinimumStorage%20RayTriangle%20Intersection.pdf
static TracingResult ray_checkTriangleIntersectionWithCulling(Ray ray, const Triangle *t) {
    Vector3 pvec, tvec, qvec;
    TracingResult result;
    result.hit = 0;
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
//    u *= inv_det; // may be useful in the future to get texture coordinates
//    v *= inv_det;
    result.distance = d;
    result.hit = 1;
    return result;
}

//static TracingResult ray_checkTriangleIntersectionNoCulling(Ray ray, const Triangle *t) {
//    TracingResult result;
//    result.hit = 0;
//    Vector3 pvec, tvec, qvec;
//    VEC3_CROSS(pvec, ray.direction, t->edges[1]);
//    double det = VEC3_DOT(t->edges[0], pvec);
//#define EPSILON 0.000001
//    if (det > -EPSILON && det < EPSILON) {
//        return result;
//    }
//#undef EPSILON
//    double inv_det = 1.0 / det;
//    VEC3_SUB(tvec, ray.origin, t->a);
//    double u = VEC3_DOT(tvec, pvec) * inv_det;
//    if (u < 0.0 || u  > 1.0) {
//        return result;
//    }
//    VEC3_CROSS(qvec, tvec, t->edges[0]);
//    double v = VEC3_DOT(ray.direction, qvec) * inv_det;
//    if (v < 0.0 || u + v > 1.0) {
//        return result;
//    }
//    double d = VEC3_DOT(t->edges[1], qvec) * inv_det;
//    result.distance = d;
//    result.hit = 1;
//    return result;
//}


Color getHighlightedColor(Color color, ShadingResult highlight, double ambientCoef) {
    double diffusedCoef = 1.0 - ambientCoef;
    color = color_mult(color, ambientCoef + highlight.diffused * diffusedCoef);
    color = color_add(color, color_mult(COLOR_WHITE, highlight.specular));
    return color;
}
