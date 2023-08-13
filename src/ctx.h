/*
** Context is the initialization data, and data that
** has to persist between frames
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "gl.h"
#include <GLFW/glfw3.h>

#include "glm/ext.hpp"

#include "buffers.h"
#include "ctx_init.h"
#include "shader.h"
#include "camera.h"
#include "texture.h"
#include "mesh.h"
#include "mesh_samples.h"

typedef struct {
  int should_continue;
  GLFWwindow* window;
  shader_t* default_shader;
  shader_t* light_shader;
  shader_t* sky_shader;

  mesh_t*   pyramid;
  glm::mat4 pyramid_transform;

  mesh_t* floor;

  mesh_t* lamp;
  
  camera_t* camera;
} ctx_t;

/*
 * Constructor, it is called before main loop starts
 * It initializes the video, and creates the window too.
 */
inline static void ctx_resize_framebuffer_to_window(
  ctx_t* c, int* width, int* height
);

ctx_t* ctx_create() {
  ctx_init_video();

  ctx_t* ctx = (ctx_t*) malloc(sizeof(ctx_t));
  ctx->should_continue = 1;
  ctx->window = ctx_init_window(320, 240);

  int width, height; // Actuall values can be doubled on Apple
  ctx_resize_framebuffer_to_window(ctx, &width, &height);

  ctx->default_shader = shader_create("src/shaders/default.vert", "src/shaders/default.frag");
  ctx->light_shader   = shader_create("src/shaders/light.vert", "src/shaders/light.frag");
  ctx->sky_shader     = shader_create("src/shaders/default.vert", "src/shaders/sky.frag");

  glm::vec4 light_color = glm::vec4(1.0f, 1.0f, 1.0f, 0.5f);
  glm::vec3 light_pos   = glm::vec3(1.0f, 1.0f, 0.5f);
  glm::mat4 lamp_transform = translate(glm::mat4(1.0f), light_pos);
  ctx->lamp                = mesh_sample_create_lamp();

  ctx->pyramid_transform = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5, -0.5, 0.0f));
  ctx->pyramid           = mesh_sample_create_pyramid();

  ctx->floor = mesh_sample_create_floor();

  glm::mat4 skybox_transform = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5, -0.5, 0.0f));

	shader_activate(ctx->light_shader);
	glUniformMatrix4fv(glGetUniformLocation(ctx->light_shader->ID, "modelToWorld"), 1, GL_FALSE, glm::value_ptr(lamp_transform));
  glUniform4f(glGetUniformLocation(ctx->light_shader->ID, "lightColor"), light_color[0], light_color[1], light_color[2], light_color[3]);

  shader_activate(ctx->default_shader);
	glUniformMatrix4fv(glGetUniformLocation(ctx->default_shader->ID, "modelToWorld"), 1, GL_FALSE, glm::value_ptr(ctx->pyramid_transform));
	glUniform4f(glGetUniformLocation(ctx->default_shader->ID, "lightColor"), light_color[0], light_color[1], light_color[2], light_color[3]);
  glUniform3f(glGetUniformLocation(ctx->default_shader->ID, "lightPos"), light_pos[0], light_pos[1], light_pos[2]);

  shader_activate(ctx->sky_shader);
	glUniformMatrix4fv(glGetUniformLocation(ctx->sky_shader->ID, "modelToWorld"), 1, GL_FALSE, glm::value_ptr(skybox_transform));
	glUniform4f(glGetUniformLocation(ctx->default_shader->ID, "lightColor"), light_color[0], light_color[1], light_color[2], light_color[3]);
  glUniform3f(glGetUniformLocation(ctx->default_shader->ID, "lightPos"), light_pos[0], light_pos[1], light_pos[2]);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);

  ctx->camera = camera_create(width, height, glm::vec3(2.0f, 1.8f, 3.0f));

  return ctx;
}

inline static void ctx_resize_framebuffer_to_window(
  ctx_t* ctx, int* width, int* height
) {
  // - gets width and height into pointers passed as args
  // - uses them bellow in this function
  // - and also "returns" them as references
  glfwGetFramebufferSize(ctx->window, width, height);

  glViewport(0, 0, *width, *height);
}

/*
 * Performs one iteration of the main loop.
 *
 * - Checks for user input;
 * - updates application state accordingly,
 * - and triggers rendering if necessary.
 */
static void ctx_handle_input(ctx_t* ctx);
static void ctx_advance_state(ctx_t* ctx);
static void ctx_render(ctx_t* ctx);

void ctx_perform_one_cycle(ctx_t* ctx) {
  static int frames_rendered = 0;
  static double last_fps_print_time = 0.0;
  double current_time = glfwGetTime();
  
  // Calculate the elapsed time since the last FPS calculation
  double time_since_last_fps_print = current_time - last_fps_print_time;
  
  if (time_since_last_fps_print >= 5.0) {
    // Calculate the FPS and print it to the console
    double fps = frames_rendered / time_since_last_fps_print;
    
    // Reset the frame counter and update the last FPS print time
    frames_rendered = 0;
    last_fps_print_time = current_time;

    // This makes OpenGL to adjust to window side changes
    // (Important for web, but desktop somehow already handles it)
    int width, height;
    ctx_resize_framebuffer_to_window(ctx, &width, &height);
    printf("%dx%d at %.1f FPS\n", width, height, fps);
  }
  
  // Handle input, advance state, and render the scene
  ctx_handle_input(ctx);
  ctx_advance_state(ctx);
  ctx_render(ctx);
  
  // Poll events from the window system's event queue
  glfwPollEvents();
  
  // Increment the frame counter
  frames_rendered++;
}

inline static void ctx_handle_input(ctx_t* ctx) {
  if (glfwGetKey(ctx->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(ctx->window, GLFW_TRUE);
  }
  if (glfwWindowShouldClose(ctx->window)) {
    ctx->should_continue = 0;
  }

  camera_inputs(ctx->camera, ctx->window);
}

inline static void ctx_advance_state(ctx_t* ctx) {
  camera_update_matrix(ctx->camera, 45.0f, 0.1f, 100.0f);
}

inline static void ctx_render(ctx_t* ctx) {
    // Specify the color of the background

    glClearColor(0.4f, 0.33f, 0.17f, 1.0f);
    // Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader_activate(ctx->light_shader);
    mesh_draw(ctx->lamp, ctx->light_shader, ctx->camera);

    shader_activate(ctx->default_shader);
  	glUniformMatrix4fv(glGetUniformLocation(ctx->default_shader->ID, "modelToWorld"), 1, GL_FALSE, (GLfloat*)glm::value_ptr(ctx->pyramid_transform));
    mesh_draw(ctx->pyramid, ctx->default_shader, ctx->camera);

    mesh_draw(ctx->pyramid, ctx->default_shader, ctx->camera);
    mesh_draw(ctx->floor,   ctx->default_shader, ctx->camera);

    // Swap the back buffer with the front buffer
    glfwSwapBuffers(ctx->window);
}

/*
 * Destructor exits graphics and frees memory
 */
void ctx_delete(ctx_t* ctx) {
  printf("Destroying context...\n");
	glfwDestroyWindow(ctx->window);
	glfwTerminate();

  free(ctx);
}