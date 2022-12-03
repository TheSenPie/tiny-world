#include "cylinder.h"
#include "../util/fmath.h"

// N = number of sectors
// S = number of stacks
void cylinder_init (struct Cylinder *self, f32 radius, f32 height, int N, int S) {
    self->N = N;
    self->S = S;
    self->top_vertices_size = (N+1) * 9;
    self->top_indices_size = (N+2);
    self->bottom_vertices_size = (N+1) * 9;
    self->bottom_indices_size = (N+2);
    self->side_vertices_size = (N+1) * (S+1) * 9;
    self->side_indices_size = S * (N + 1) * 2 + 2 * (S - 1); // take into account degenrate points
    int vert_size = self->top_vertices_size + self->bottom_vertices_size + self->side_vertices_size;
    self->vertices = malloc(vert_size * sizeof(f32));
    int ind_size = self->top_indices_size + self->bottom_indices_size + self->side_indices_size;
    self->indices = malloc(ind_size * sizeof(int));

    // init the vertices
    int v_offset = 0, i_offset = 0;

    make_top_fan(self, radius, height, N, &v_offset, &i_offset);
    make_bottom_fan(self, radius, height, N, &v_offset, &i_offset);
    make_sides(self, radius, height, N, S, &v_offset, &i_offset);
}

void make_top_fan (struct Cylinder *self, f32 radius, f32 height, int N, int *v_offset, int *i_offset) {
    int v_idx = *v_offset, i_idx = *i_offset;
    // xyz
    self->vertices[v_idx + 0] = 0;
    self->vertices[v_idx + 1] = 0.5f * height;
    self->vertices[v_idx + 2] = 0;

    // n_xyz
    self->vertices[v_idx + 3] = 0;
    self->vertices[v_idx + 4] = 1;
    self->vertices[v_idx + 5] = 0;

    // rgb
    self->vertices[v_idx + 6] = 1;
    self->vertices[v_idx + 7] = 0;
    self->vertices[v_idx + 8] = 0;

    v_idx += 9;

    for (; v_idx < *v_offset + self->top_vertices_size; v_idx += 9) {
        f32 x, y, z;
        x = sinf((float) ( (v_idx - 9) / 9) / (float) N * TAU);
        y = 0.5f;
        z = cosf((float) ( (v_idx - 9) / 9) / (float) N * TAU);

        f32 r, g, b;
        r = 1;
        g = 0;
        b = 0;

        f32 n_x, n_y, n_z;
        n_x = 0;
        n_y = 1;
        n_z = 0;

        self->vertices[v_idx] = radius * x;
        self->vertices[v_idx + 1] = height * y;
        self->vertices[v_idx + 2] = radius * z;
        self->vertices[v_idx + 3] = n_x;
        self->vertices[v_idx + 4] = n_y;
        self->vertices[v_idx + 5] = n_z;
        self->vertices[v_idx + 6] = r;
        self->vertices[v_idx + 7] = g;
        self->vertices[v_idx + 8] = b;
    }

    // init the indices
    for (; i_idx < *i_offset + self->top_indices_size; i_idx++) {
        self->indices[i_idx] = i_idx;
    }

    self->indices[i_idx - 1] = self->indices[*i_offset + 1];

    // increase the offsets
    *v_offset += self->top_vertices_size;
    *i_offset += self->top_indices_size;
}

void make_bottom_fan (struct Cylinder *self, f32 radius, f32 height, int N, int *v_offset, int *i_offset) {
    int v_idx = *v_offset, i_idx = *i_offset;
    // xyz
    self->vertices[v_idx + 0] = 0;
    self->vertices[v_idx + 1] = 0.5f * height * -0.1f;
    self->vertices[v_idx + 2] = 0;

    // n_xyz
    self->vertices[v_idx + 3] = 0;
    self->vertices[v_idx + 4] = -1;
    self->vertices[v_idx + 5] = 0;

    // rgb
    self->vertices[v_idx + 6] = 1;
    self->vertices[v_idx + 7] = 0;
    self->vertices[v_idx + 8] = 0;

    v_idx += 9;

    for (; v_idx < *v_offset + self->bottom_vertices_size; v_idx += 9) {
        f32 x, y, z;
        x = sinf((float) ( (v_idx - 9) / 9) / (float) N * TAU);
        y = 0.5f;
        z = cosf((float) ( (v_idx - 9) / 9) / (float) N * TAU);

        f32 r, g, b;
        r = 1;
        g = 0;
        b = 0;

        f32 n_x, n_y, n_z;
        n_x = 0;
        n_y = -1;
        n_z = 0;

        self->vertices[v_idx] = radius * x;
        self->vertices[v_idx + 1] = height * y * -1.0f;
        self->vertices[v_idx + 2] = radius * z;
        self->vertices[v_idx + 3] = n_x;
        self->vertices[v_idx + 4] = n_y;
        self->vertices[v_idx + 5] = n_z;
        self->vertices[v_idx + 6] = r;
        self->vertices[v_idx + 7] = g;
        self->vertices[v_idx + 8] = b;
    }

    // init the indices
    for (; i_idx < *i_offset + self->bottom_indices_size; i_idx++) {
        self->indices[i_idx] = i_idx;
    }

    self->indices[i_idx - 1] = self->indices[*i_offset + 1];

    // increase the offsets
    *v_offset += self->bottom_vertices_size;
    *i_offset += self->bottom_indices_size;
}

void make_sides (struct Cylinder *self, f32 radius, f32 height, int N, int S, int *v_offset, int *i_offset) {
    int v_idx = *v_offset, i_idx = *i_offset;
    int s, n;
    for (s = 0; s <= S; s++) {
        for (n = N; n >= 0; n -= 1) {
            f32 x, y, z;
            x = sinf((float) n / (float) N * TAU);
            y = (float) (s / (float) S - 0.5f);
            z = cosf((float) n / (float) N * TAU);

            f32 r, g, b;
            r = sinf(((float) n / (float) N + 0) * TAU);
            g = sinf(((float) n / (float) N + 0.33) * TAU);
            b = sinf(((float) n / (float) N + 0.66) * TAU);

            f32 n_x, n_y, n_z;
            n_x = x;
            n_y = 0;
            n_z = z;

            self->vertices[v_idx] = radius * x;
            self->vertices[v_idx + 1] = height * y;
            self->vertices[v_idx + 2] = radius * z;
            self->vertices[v_idx + 3] = n_x;
            self->vertices[v_idx + 4] = n_y;
            self->vertices[v_idx + 5] = n_z;
            self->vertices[v_idx + 6] = r;
            self->vertices[v_idx + 7] = g;
            self->vertices[v_idx + 8] = b;

            // printf("n: %d x: %f, y: %f, z: %f\n", v_idx / 9, self->vertices[v_idx], self->vertices[v_idx + 1], self->vertices[v_idx + 2]);

            v_idx += 9;
        }
    }

    // init the indices
    for (s = 0; s < S; s++) {
        for (n = 0; n <= N; n++) {
            // quad
            self->indices[i_idx]         = *v_offset / 9 + s * (N + 1) + n;
            self->indices[i_idx + 1]     = *v_offset / 9 + (s + 1) * (N + 1) + n;

            // printf(", %d, %d\n", self->indices[i_idx], self->indices[i_idx + 1]);
            // printf(", up   i: %d  x: %f y: %f z: %f\n", self->indices[i_idx], self->vertices[self->indices[i_idx] * 9], self->vertices[self->indices[i_idx] * 9 + 1], self->vertices[self->indices[i_idx] * 9 + 2]);
            // printf(", down i: %d  x: %f y: %f z: %f\n", self->indices[i_idx + 1], self->vertices[self->indices[i_idx + 1] * 9], self->vertices[self->indices[i_idx + 1] * 9 + 1], self->vertices[self->indices[i_idx + 1] * 9 + 2]);
            i_idx += 2;
        }
        if (s + 1 < S) {
            // printf("generated degenerate\n");
            // add indices for degenrate triangles
            self->indices[i_idx]        = *v_offset / 9 + (s + 1) * (N + 1) + (N - 1) + 1;
            self->indices[i_idx + 1]    = *v_offset / 9 + (s + 1) * (N + 1);
            // printf(", %d, %d\n", self->indices[i_idx], self->indices[i_idx + 1]);
            i_idx += 2;
        }
    }

    *v_offset += self->side_vertices_size;
    *i_offset += self->side_indices_size;
}