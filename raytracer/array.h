//
//  array.h
//  raytracer
//
//  Created by Krzysztof Gabis on 01.04.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#ifndef raytracer_array_h
#define raytracer_array_h

#define ARRAY_GET(a, index) ((void*)((a)->items + ((index) * (a)->itemSize)))

typedef struct {
    unsigned char *items;
    unsigned int itemSize;
    unsigned int count;
    unsigned int capacity;
} Array;

Array *array_init(Array *array, unsigned int itemSize, unsigned int initialCapacity);
int array_add(Array *array, void *item); // 1 on success
int array_addArray(Array *array, const Array *toAdd);
void* array_getSafe(const Array *a, unsigned int index);
void* array_get(const Array *array, unsigned int index);
void array_remove(Array *array, unsigned int index);
void array_dealloc(Array *array);

#endif
