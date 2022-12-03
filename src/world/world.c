#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-braces"

#include <cglm/struct.h>

#pragma GCC diagnostic pop

#include "world.h"

void world_init (struct World *self) {
    self->cube = (struct Cube) {GLMS_VEC3_ZERO_INIT};
    self->light = (struct Light) { (vec3s) {{1.2f, 0.0f, 2.0f}} };
    cylinder_init(&(self->cylinder), 1, 4, 100, 4);
}