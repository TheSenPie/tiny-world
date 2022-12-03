#include "gfx/window.h"
#include "world/world.h"
#include "gfx/gfx.h"

#include "state.h"

// global state
struct State state;

void init() {
	state.window = &window;
	renderer_init(&state.renderer);
	world_init(&state.world);
	mouse_set_grabbed(true);
}

void destroy() {
	renderer_destroy(&state.renderer);
}

void tick() {
	state.ticks++;
}

void update() {
	state.world.light.position.x = sin(glfwGetTime()) * 2.0f;;
	state.world.light.position.z = cos(glfwGetTime()) * 2.0f;

	if (state.window->mouse.buttons[GLFW_MOUSE_BUTTON_LEFT].down) {
		state.renderer.perspective_camera.theta -= state.window->mouse.delta.x * 0.01f;
		state.renderer.perspective_camera.fi += state.window->mouse.delta.y * 0.01f;

		perspective_camera_look_at(&state.renderer.perspective_camera, state.world.cube.position);
	}
	
    // wireframe toggle (T)
    if (state.window->keyboard.keys[GLFW_KEY_T].pressed) {
        state.renderer.flags.wireframe = !state.renderer.flags.wireframe;
    }

    // mouse toggle (ESC)
    if (state.window->keyboard.keys[GLFW_KEY_ESCAPE].pressed) {
        mouse_set_grabbed(!mouse_get_grabbed());
    }
}

void render() {
	renderer_prepare(&state.renderer);
	mat4s cube_model = GLMS_MAT4_IDENTITY;
	cube_model = glms_translate(cube_model, state.world.cube.position);
	renderer_cube_color(&state.renderer, cube_model, state.world.light.position, SHADER_BASIC_SHADED);

	mat4s light_model = GLMS_MAT4_IDENTITY;
	light_model = glms_translate(light_model, state.world.light.position);
	light_model = glms_scale_uni(light_model, 0.3f);
	renderer_cube_color(&state.renderer, light_model, state.world.light.position, SHADER_LIGHT_CUBE);

	renderer_cylinder_color(&state.renderer, &state.world.cylinder, state.world.light.position);
}

int main(int argc, char *argv[]) {
	window_create(init, destroy, tick, update, render);
	window_loop();
	return 0;
}
