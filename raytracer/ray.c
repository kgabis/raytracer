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

#define SQUARE(x) ((x)*(x))
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define MAX_VISIBLE_DISTANCE 1000.0

Ray ray_makeForPixel(Camera *c, size_t x, size_t y) {
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

TracingResult ray_trace(Ray ray, Scene *scene) {
    TracingResult closestHit;
    TracingResult currentHit;
    closestHit.hit = 0;
    closestHit.distance = 1.0/0.0; // infinity
    for (size_t i = 0; i < scene->objects.count; i++) {
        currentHit = ray_checkIntersection(ray, array_get(&scene->objects, i));
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
