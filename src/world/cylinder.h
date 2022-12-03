#ifndef CYLINDER
#define CYLINDER
#include "../util/types.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-braces"

#include <cglm/struct.h>

#pragma GCC diagnostic pop

struct Cylinder {
    vec3s postion;
    int S; // number of stacks
    int N; // number of sectors
    
    int side_vertices_size;
    int side_indices_size;
    int top_vertices_size;
    int top_indices_size;
    int bottom_vertices_size;
    int bottom_indices_size;

    float *vertices;
    int *indices;
};

void cylinder_init (struct Cylinder *self, f32 radius, f32 height, int N, int S);
void make_top_fan (struct Cylinder *self, f32 radius, f32 height, int N, int *v_offset, int *i_offset);
void make_bottom_fan (struct Cylinder *self, f32 radius, f32 height, int N, int *v_offset, int *i_offset);
void make_sides (struct Cylinder *self, f32 radius, f32 height, int N, int S, int *v_offset, int *i_offset);
#endif