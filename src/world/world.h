#include "cube.h"
#include "light.h"
#include "cylinder.h"

struct World {
    struct Cube cube;
    struct Light light;
    struct Cylinder cylinder;
};

void world_init (struct World *self);