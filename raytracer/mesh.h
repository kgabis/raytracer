//
//  mesh.h
//  raytracer
//
//  Created by Krzysztof Gabis on 08.04.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#ifndef raytracer_mesh_h
#define raytracer_mesh_h

#include "array.h"
#include "material.h"

Array mesh_load(const char *filename, Material material);

#endif
