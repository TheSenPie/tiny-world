#include "stdio.h"

#include "renderer.h"

void renderer_init(struct Renderer *self) {
    *self = (struct Renderer) {0};
    self->camera_type = CAMERA_PERSPECTIVE;
    self->current_shader = SHADER_NONE;

    self->shaders[SHADER_BASIC_COLOR] = shader_create(
        "res/shaders/basic_color.vs", "res/shaders/basic_color.fs",
        1, (struct VertexAttr[]) {
            { .index = 0, .name = "position" }
        }
    );

    self->shaders[SHADER_BASIC_SHADED] = shader_create(
        "res/shaders/basic_shaded.vs", "res/shaders/basic_shaded.fs",
        1, (struct VertexAttr[]) {
            { .index = 0, .name = "aPos" },
            { .index = 1, .name = "aNormal" },
            { .index = 2, .name = "aColor" }
        }
    );

    self->shaders[SHADER_LIGHT_CUBE] = shader_create(
        "res/shaders/light_cube.vs", "res/shaders/light_cube.fs",
        1, (struct VertexAttr[]) {
            { .index = 0, .name = "aPos" },
            { .index = 1, .name = "aNormal" },
            { .index = 2, .name = "aColor" }
        }
    );

    self->vao = vao_create();
    self->vbo = vbo_create(GL_ARRAY_BUFFER, false);
    self->ibo = vbo_create(GL_ELEMENT_ARRAY_BUFFER, false);

    perspective_camera_init(&self->perspective_camera, radians(75.0f));

    // set clear color
    self->clear_color.r = 0.2f;
    self->clear_color.g = 0.3f;
    self->clear_color.b = 0.3f;
    self->clear_color.a = 1.0f;
}

void renderer_destroy(struct Renderer *self) {
    for (size_t i = 0; i <= SHADERS_LAST; i++) {
        shader_destroy(self->shaders[i]);
    }

    vao_destroy(self->vao);
    vbo_destroy(self->vbo);
    vbo_destroy(self->ibo);
}

void renderer_prepare(struct Renderer *self) {
    glClearColor(
        self->clear_color.x, self->clear_color.y,
        self->clear_color.z, self->clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, self->flags.wireframe ? GL_LINE : GL_FILL);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void renderer_set_camera(struct Renderer *self, enum CameraType type) {
    self->camera_type = type;
}

void renderer_set_view_proj(struct Renderer *self) {
    struct ViewProj view_proj;

    switch (self->camera_type) {
        case CAMERA_PERSPECTIVE:
            view_proj = self->perspective_camera.view_proj;
            break;
        case CAMERA_ORTHO:
            view_proj = self->ortho_camera.view_proj;
            break;
    }

    shader_uniform_view_proj(self->shader, view_proj);
}

void renderer_use_shader(struct Renderer *self, enum ShaderType shader) {
    if (shader == self->current_shader) {
        return;
    }

    self->current_shader = shader;
    self->shader = self->shaders[shader];
    shader_bind(self->shader);
}

void renderer_quad_color(
    struct Renderer *self, vec2s size,
    vec4s color, mat4s model) {
    renderer_use_shader(self, SHADER_BASIC_COLOR);
    renderer_set_view_proj(self);
    shader_uniform_mat4(self->shader, "m", model);
    shader_uniform_vec4(self->shader, "color", color);

    vbo_buffer(self->vbo, (f32[]) {
        0, 0, 2,
        0, size.y, 2,
        size.x, size.y, 2,
        size.x, 0, 2,
    }, 0, (4 * 3) * sizeof(f32));

    vbo_buffer(self->ibo, (u32[]) {
        3, 0, 1, 3, 1, 2
    }, 0, 6 * sizeof(u32));

    vao_attr(self->vao, self->vbo, 0, 3, GL_FLOAT, 0, 0);

    vao_bind(self->vao);
    vbo_bind(self->ibo);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *) 0);
}

void renderer_cube_color(struct Renderer *self, mat4s model, vec3s light_pos, enum ShaderType shader) {
    vao_bind(self->vao);
    vbo_bind(self->vbo);
    vbo_bind(self->ibo);

    renderer_use_shader(self, shader);
    renderer_set_view_proj(self);
    shader_uniform_mat4(self->shader, "m", model);
    shader_uniform_vec3(self->shader, "lightColor", (vec3s) {{1.0f, 1.0f, 1.0f}});
    shader_uniform_vec3(self->shader, "lightPos", light_pos);

    // vertex position array
    GLfloat vertices[]  = {
        // position         // normal   // color
        .5f, .5f, .5f,      0, 0, 1,    1, 1, 1,   
        -.5f, .5f, .5f,     0, 0, 1,    1, 1, 0,   
        -.5f,-.5f, .5f,     0, 0, 1,    1, 0, 0,   
        .5f,-.5f, .5f,      0, 0, 1,    1, 0, 1,
        // v0,v1,v2,v3 (front)
        
        // position         // normal   // color
        .5f, .5f, .5f,      1, 0, 0,    1, 1, 1,   
        .5f,-.5f, .5f,      1, 0, 0,    1, 0, 1,   
        .5f,-.5f,-.5f,      1, 0, 0,    0, 0, 1,   
        .5f, .5f,-.5f,      1, 0, 0,    0, 1, 1,  
        // v0,v3,v4,v5 (right)
        
        // position         // normal   // color
        .5f, .5f, .5f,      0, 1, 0,    1, 1, 1,   
        .5f, .5f,-.5f,      0, 1, 0,    0, 1, 1,   
        -.5f, .5f,-.5f,     0, 1, 0,    0, 1, 0,   
        -.5f, .5f, .5f,     0, 1, 0,    1, 1, 0,  
        // v0,v5,v6,v1 (top)

        // position         // normal   // color
        -.5f, .5f, .5f,     -1, 0, 0,   1, 1, 0,   
        -.5f, .5f,-.5f,     -1, 0, 0,   0, 1, 0,   
        -.5f,-.5f,-.5f,     -1, 0, 0,   0, 0, 0,   
        -.5f,-.5f, .5f,     -1, 0, 0,   1, 0, 0,  
        // v1,v6,v7,v2 (left)

        // position         // normal   // color
        -.5f,-.5f,-.5f,     0,-1, 0,    0, 0, 0,   
        .5f,-.5f,-.5f,      0,-1, 0,    0, 0, 1,   
        .5f,-.5f, .5f,      0,-1, 0,    1, 0, 1,   
        -.5f,-.5f, .5f,     0,-1, 0,    1, 0, 0,  
        // v7,v4,v3,v2 (bottom)

        // position         // normal   // color
        .5f,-.5f,-.5f,      0, 0,-1,    0, 0, 1,   
        -.5f,-.5f,-.5f,     0, 0,-1,    0, 0, 0,   
        -.5f, .5f,-.5f,     0, 0,-1,    0, 1, 0,   
        .5f, .5f,-.5f,      0, 0,-1,    0, 1, 1,
        // v4,v7,v6,v5 (back)
    };

    vbo_buffer(self->vbo, vertices, 0, (9 * 4 * 6) * sizeof(f32));

    vbo_buffer(self->ibo, (u32[]) {
         0, 1, 2,   2, 3, 0,    // v0-v1-v2, v2-v3-v0 (front)
         4, 5, 6,   6, 7, 4,    // v0-v3-v4, v4-v5-v0 (right)
         8, 9,10,  10,11, 8,    // v0-v5-v6, v6-v1-v0 (top)
        12,13,14,  14,15,12,    // v1-v6-v7, v7-v2-v1 (left)
        16,17,18,  18,19,16,    // v7-v4-v3, v3-v2-v7 (bottom)
        20,21,22,  22,23,20     // v4-v7-v6, v6-v5-v4 (back)
    }, 0, 36 * sizeof(u32));

    vao_attr(self->vao, self->vbo, 0, 3, GL_FLOAT, 9 * sizeof(GLfloat), 0);
    vao_attr(self->vao, self->vbo, 1, 3, GL_FLOAT, 9 * sizeof(GLfloat), (3 * sizeof(GLfloat)));
    vao_attr(self->vao, self->vbo, 2, 3, GL_FLOAT, 9 * sizeof(GLfloat), (6 * sizeof(GLfloat)));

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void *) 0);
}

void renderer_cylinder_color(struct Renderer *self, struct Cylinder *cylinder, vec3s light_pos) {
    vao_bind(self->vao);
    vbo_bind(self->vbo);
    vbo_bind(self->ibo);

    renderer_use_shader(self, SHADER_BASIC_SHADED);
    renderer_set_view_proj(self);
    shader_uniform_mat4(self->shader, "m", GLMS_MAT4_IDENTITY);
    shader_uniform_vec3(self->shader, "lightColor", (vec3s) {{1.0f, 1.0f, 1.0f}});
    shader_uniform_vec3(self->shader, "lightPos", light_pos);

    // printf("AAAAAAA: %d\n", cylinder->vertices_size);
    int vert_size = cylinder->top_vertices_size + cylinder->bottom_vertices_size + cylinder->side_vertices_size;
    vbo_buffer(self->vbo, cylinder->vertices, 0, vert_size * sizeof(GLfloat));

    // printf("BBBBBBB: %d\n", cylinder->indices_size);
    int ind_size = cylinder->top_indices_size + cylinder->bottom_indices_size + cylinder->side_indices_size;
    vbo_buffer(self->ibo, cylinder->indices, 0, ind_size * sizeof(int));

    vao_attr(self->vao, self->vbo, 0, 3, GL_FLOAT, 9 * sizeof(GLfloat), 0);
    vao_attr(self->vao, self->vbo, 1, 3, GL_FLOAT, 9 * sizeof(GLfloat), (3 * sizeof(GLfloat)));
    vao_attr(self->vao, self->vbo, 2, 3, GL_FLOAT, 9 * sizeof(GLfloat), (6 * sizeof(GLfloat)));

    // printf("\n\n");
    // int n;
    // for (n = cylinder->top_indices_size + cylinder->bottom_indices_size; n < cylinder->top_indices_size + cylinder->bottom_indices_size + cylinder->side_indices_size; n++) {
    //     printf("element: %d n: %d\n", cylinder->indices[n], n);
    // }
    // printf("\n\n");

    // printf("\n\n");
    // for (n = cylinder->top_vertices_size + cylinder->bottom_vertices_size; n < cylinder->top_vertices_size + cylinder->bottom_vertices_size + cylinder->side_vertices_size; n += 9) {
    //     printf("x: %f, y: %f, z: %f n: %d\n", cylinder->vertices[n], cylinder->vertices[n + 1], cylinder->vertices[n + 2], n / 9);
    // }
    // printf("\n\n");

    glDrawElements(GL_TRIANGLE_FAN, cylinder->top_indices_size, GL_UNSIGNED_INT, (void *) 0);
    glDrawElements(GL_TRIANGLE_FAN, cylinder->bottom_indices_size, GL_UNSIGNED_INT, (void *) (cylinder->top_indices_size * sizeof(int)));
    glDrawElements(GL_TRIANGLE_STRIP, cylinder->side_indices_size, GL_UNSIGNED_INT, (void *) ((cylinder->top_indices_size + cylinder->bottom_indices_size) * sizeof(int)));
}
