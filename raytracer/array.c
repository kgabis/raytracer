//
//  array.c
//  raytracer
//
//  Created by Krzysztof Gabis on 01.04.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#include <stdlib.h>
#include <string.h>
#include "array.h"

Array *array_init(Array *a, size_t itemSize, size_t initialCapacity) {
    if (initialCapacity == 0) {
        initialCapacity = 1; // Idiot-proof
    }
    a->items = malloc(itemSize * initialCapacity);
    if (a->items == NULL) {
        free(a);
        return NULL;
    }
    a->itemSize = itemSize;
    a->capacity = initialCapacity;
    a->count = 0;
    return a;
}

int array_add(Array *a, void *item) {
    void *reallocPtr;
    size_t newCapacity, memOffset;
    if (a->count >= a->capacity) {
        newCapacity = a->capacity * 2;
        reallocPtr = realloc(a->items, newCapacity * a->itemSize);
        if (reallocPtr == NULL) {
            return 0;
        }
        a->items = reallocPtr;
        a->capacity = newCapacity;
    }
    memOffset = (a->count * a->itemSize);
    memmove(a->items + memOffset, item, a->itemSize);
    a->count++;
    return 1;
}

void* array_get(Array *a, size_t index) {
    void *item;
    size_t memOffset;
    if (index >= a->count) {
        return NULL;
    }
    memOffset = (index * a->itemSize);
    item = a->items + memOffset;
    return item;
}

size_t array_getCount(Array *a) {
    return a->count;
}

void array_remove(Array *a, size_t index) {
    void *itemToRemove, *itemToMove;
    if (index >= a->count) {
        return;
    }
    if (a->count == 1) {
        a->count--;
        return;
    }
    itemToRemove = a->items + ((index * a->itemSize) / sizeof(void*));
    itemToMove = a->items + ((a->count * a->itemSize) / sizeof(void*));
    memmove(itemToRemove, itemToMove, a->itemSize);
    a->count--;
}

void array_dealloc(Array *a) {
    free(a);
}
