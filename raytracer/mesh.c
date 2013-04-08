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
#include "object.h"
#include "utils.h"

#define LINE_SIZE 128

enum FileStatus {
    FSHeader,
    FSVertices,
    FSFaces
};

Array mesh_load(const char *filename, Material material) {
    Array vertices, triangles;
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        goto fp_error;
    }
    char line[LINE_SIZE];
    array_init(&vertices, sizeof(Vector3), 1500);
    array_init(&triangles, sizeof(Object), 1500);
    double x, y, z;
    size_t verticeNums[3];
    Vector3 *faceVertices[3];
    int parseStatus = FSHeader;
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
                    sscanf(line, "%lf %lf %lf\n", &x, &y, &z);
                    Vector3 v = vec3_make(x, y, z);
                    array_add(&vertices, &v);
                }
                break;
            case FSFaces:
                sscanf(line, "%zu %zu %zu\n", &verticeNums[0], &verticeNums[1], &verticeNums[2]);
                faceVertices[0] = ARRAY_GET(&vertices, verticeNums[0]);
                faceVertices[1] = ARRAY_GET(&vertices, verticeNums[1]);
                faceVertices[2] = ARRAY_GET(&vertices, verticeNums[2]);
                Object face = object_initTriangle(*faceVertices[0], *faceVertices[1], *faceVertices[2], material);
                array_add(&triangles, &face);
                break;
            default:
                goto error;
                break;
        }
    }
    fclose(fp);
    printf("Vertices:  %zd\n", vertices.count);
    printf("Triangles: %zd\n", triangles.count);
    return triangles;
error:
    fclose(fp);
fp_error:
    array_dealloc(&vertices);
    array_dealloc(&triangles);
    array_init(&triangles, sizeof(Object), 1);
    return triangles;
}
