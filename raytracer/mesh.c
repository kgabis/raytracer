//
//  mesh.c
//  raytracer
//
//  Created by Krzysztof Gabis on 08.04.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#include <stdio.h>
#include <string.h>

#include "mesh.h"
#include "surface.h"
#include "utils.h"

#define LINE_SIZE 128

enum FileStatus {
    FSHeader,
    FSVertices,
    FSFaces
};

Array mesh_load(const char *filename, Material material) {
    Array vertices, triangles;
    FILE *fp;
    char line[LINE_SIZE];
    array_init(&vertices, sizeof(Vector3), 1500);
    array_init(&triangles, sizeof(Surface), 1500);
    double x, y, z;
    size_t vNums[3];
    Vector3 *face[3];
    int parseStatus = FSHeader;
    int sscanfResult;
    Surface faceSurface;
    fp = fopen(filename, "r");
    if (fp == NULL) {
        goto fp_error;
    }
    while (fgets(line, LINE_SIZE, fp) != NULL) {
        switch (parseStatus) {
            case FSHeader:
                if (STREQ(line, "END_HEADER\n")) {
                    parseStatus = FSVertices;
                }
                break;
            case FSVertices:
                if (STREQ(line, "\n")) {
                    parseStatus = FSFaces;
                } else {
                    sscanfResult = sscanf(line, "%lf %lf %lf\n", &x, &y, &z);
                    if (sscanfResult == EOF) {
                        goto error;
                    }
                    Vector3 v = vec3_make(x, y, z);
                    array_add(&vertices, &v);
                }
                break;
            case FSFaces:
                sscanfResult = sscanf(line, "%zu %zu %zu\n", &vNums[0], &vNums[1], &vNums[2]);
                if (sscanfResult == EOF) {
                    goto error;
                }
                face[0] = ARRAY_GET(&vertices, vNums[0]);
                face[1] = ARRAY_GET(&vertices, vNums[1]);
                face[2] = ARRAY_GET(&vertices, vNums[2]);
                faceSurface = surface_initTriangle(*face[0], *face[1], *face[2], material);
                array_add(&triangles, &faceSurface);
                break;
            default:
                goto error;
                break;
        }
    }
    fclose(fp);
    array_dealloc(&vertices);
//    printf("Vertices:  %zd\n", vertices.count);
//    printf("Triangles: %zd\n", triangles.count);
    return triangles;
error:
    fclose(fp);
fp_error:
    array_dealloc(&vertices);
    array_dealloc(&triangles);
    array_init(&triangles, sizeof(Surface), 1);
    return triangles;
}
