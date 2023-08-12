/*
** Context is the initialization data, and data that
** has to persist between frames
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "gl.h"
#include <GLFW/glfw3.h>

#include <cglm/cglm.h>

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

  mesh_t* pyramid;
  mat4 pyramidModel;

  mesh_t* lamp;
  
  camera_t* camera;
} ctx_t;

/*
 * Constructor, it is called before main loop starts
 * It initializes the video, and creates the window too.
 */
inline static void ctx_resize_framebuffer_to_window(ctx_t* c);

ctx_t* ctx_create() {
  ctx_init_video();

  ctx_t* ctx = (ctx_t*) malloc(sizeof(ctx_t));
  ctx->should_continue = 1;
  ctx->window = ctx_init_window(320, 240);

  ctx_resize_framebuffer_to_window(ctx);

  ctx->default_shader = shader_create("src/shaders/default-vert.glsl", "src/shaders/default-frag.glsl");
  ctx->light_shader = shader_create("src/shaders/light-vert.glsl", "src/shaders/light-frag.glsl");
  ctx->sky_shader = shader_create("src/shaders/default-vert.glsl", "src/shaders/sky-frag.glsl");




	vec4 lightColor;
  glm_vec4_copy((vec4){ 1.0f, 1.0f, 1.0f, 0.5f }, lightColor);

  vec3 lightPos;
	glm_vec3_copy((vec3){ 1.0f, 1.0f, 0.5f }, lightPos);
	mat4 lightModel = GLM_MAT4_IDENTITY_INIT;
  glm_translate(lightModel, lightPos);

	vec3 pyramidPos;
  glm_vec3_copy((vec3){ -1.0f, 0.0f, -1.0f }, pyramidPos);
  glm_mat4_copy((mat4)GLM_MAT4_IDENTITY_INIT, ctx->pyramidModel);
	glm_translate(ctx->pyramidModel, pyramidPos);


	vec3 skyPos;
  glm_vec3_copy((vec3){ 0.0f, 0.0f, 0.0f }, skyPos);
	mat4 skyModel = GLM_MAT4_IDENTITY_INIT;
	glm_translate(skyModel, skyPos);

  ctx->pyramid = mesh_sample_create_pyramid();
  ctx->lamp = mesh_sample_create_lamp();

	shader_activate(ctx->light_shader);
	glUniformMatrix4fv(glGetUniformLocation(ctx->light_shader->ID, "model"), 1, GL_FALSE, (GLfloat*)lightModel);
  glUniform4f(glGetUniformLocation(ctx->light_shader->ID, "lightColor"), lightColor[0], lightColor[1], lightColor[2], lightColor[3]);

  shader_activate(ctx->default_shader);
	glUniformMatrix4fv(glGetUniformLocation(ctx->default_shader->ID, "model"), 1, GL_FALSE, (GLfloat*)ctx->pyramidModel);
	glUniform4f(glGetUniformLocation(ctx->default_shader->ID, "lightColor"), lightColor[0], lightColor[1], lightColor[2], lightColor[3]);
  glUniform3f(glGetUniformLocation(ctx->default_shader->ID, "lightPos"), lightPos[0], lightPos[1], lightPos[2]);

  shader_activate(ctx->sky_shader);
	glUniformMatrix4fv(glGetUniformLocation(ctx->sky_shader->ID, "model"), 1, GL_FALSE, (GLfloat*)skyModel);
	glUniform4f(glGetUniformLocation(ctx->default_shader->ID, "lightColor"), lightColor[0], lightColor[1], lightColor[2], lightColor[3]);
  glUniform3f(glGetUniformLocation(ctx->default_shader->ID, "lightPos"), lightPos[0], lightPos[1], lightPos[2]);



  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);

  int w, h;
  glfwGetFramebufferSize(ctx->window, &w, &h);
  glViewport(0, 0, w, h);

  ctx->camera = camera_create(w, h, (vec3){2.0f, 1.8f, 3.0f});

  return ctx;
}

inline static void ctx_resize_framebuffer_to_window(ctx_t* ctx) {
  int w, h;
  glfwGetFramebufferSize(ctx->window, &w, &h);
  printf("Frame buffer size %dx%d\n", w, h);

  glViewport(0, 0, w, h);
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
    printf("FPS: %f\n", fps);
    
    // Reset the frame counter and update the last FPS print time
    frames_rendered = 0;
    last_fps_print_time = current_time;

    // This makes OpenGL to adjust to window side changes
    // (More important for web as desktop somehow already handles it)
    ctx_resize_framebuffer_to_window(ctx);
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
  	glUniformMatrix4fv(glGetUniformLocation(ctx->default_shader->ID, "model"), 1, GL_FALSE, (GLfloat*)ctx->pyramidModel);
    mesh_draw(ctx->pyramid, ctx->default_shader, ctx->camera);

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