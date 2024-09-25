#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb/stb_image.h>

#include <iostream>
#include <vector>

#include "Shader.h"

// settings
const int kWidth = 800;
const int kHeight = 600;

float fov = 45.0f;
bool key_state[512] = { 0 };

// camera
glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 camera_target = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 camera_direction = glm::normalize(camera_pos - camera_target);

// directions
glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 camera_right = glm::normalize(glm::cross(up, camera_direction));
glm::vec3 camera_up = glm::cross(camera_direction, camera_right);
glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, -1.0f);

// function prototypes
void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window, float dt);

int main()
{
	// GLFW and GLAD initialization
	// ----------------------------
	// initialize GLFW
	glfwInit();

	// GLFW window hints
	// minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// creating GLFW window
	GLFWwindow* window = glfwCreateWindow(kWidth, kHeight, "LearnOpenGL", NULL, NULL);

	// error handling for GLFW window
	const char* description;
	if (int code = glfwGetError(&description) != GLFW_NO_ERROR)
	{
		std::cout << "GLFW has failed to create a window\nGLFW error: " << description << std::endl;
		glfwTerminate();
		return -1;
	}

	// set newly created window as the OpenGL context
	// GLAD will fail if this doesn't happen
	glfwMakeContextCurrent(window);

	// set function to call on window resize
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

	// GLAD initialization and error handling
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "GLAD has failed to initialize" << std::endl;
		return -1;
	}

	// OpenGL shaders, buffers and vertex data
	// ---------------------------------------
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	glm::vec3 cube_positions[] = {
		glm::vec3( 0.0f,  0.0f,  0.0f), 
		glm::vec3( 2.0f,  5.0f, -15.0f), 
		glm::vec3(-1.5f, -2.2f, -2.5f),  
		glm::vec3(-3.8f, -2.0f, -12.3f),  
		glm::vec3( 2.4f, -0.4f, -3.5f),  
		glm::vec3(-1.7f,  3.0f, -7.5f),  
		glm::vec3( 1.3f, -2.0f, -2.5f),  
		glm::vec3( 1.5f,  2.0f, -2.5f), 
		glm::vec3( 1.5f,  0.2f, -1.5f), 
		glm::vec3(-1.3f,  1.0f, -1.5f)  
	};

	// OpenGL settings
	glEnable(GL_DEPTH_TEST);

	// shaders
	Shader shader("./vertex.glsl", "./fragment.glsl");

	// vertex array and buffers
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// load and generate texture and mipmaps
	// -------------------------------------
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// texture wrapping/filtering options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, num_channels;

	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("./container.jpg", &width, &height, &num_channels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	// free resources after use
	stbi_image_free(data);

	unsigned int transform_location = glGetUniformLocation(shader.ID, "transform");

	// render loop
	// ---------------------

	// uncomment for wireframe mode
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// for delta_time
	float prev_time = glfwGetTime();
	while (!glfwWindowShouldClose(window))
	{
		// dt calculation
		float curr_time = glfwGetTime();
		float delta_time = curr_time - prev_time;
		prev_time = curr_time;

		// process input
		ProcessInput(window, delta_time);

		// create transformations (model shader left out as it needs to be re-calculated for every cube)
		// ---------------------------------------------------------------------------------------------
		
		// projection matrix
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);

		// view matrix
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::lookAt(
			camera_pos,
			camera_pos + camera_front,
			camera_up
		);
		
		// rendering
		// ---------
		glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// activate shader
		shader.Use();

		glBindVertexArray(VAO);

		int view_loc = glGetUniformLocation(shader.ID, "view");
		glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));

		int projection_loc = glGetUniformLocation(shader.ID, "projection");
		glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection));

		for (int i = 0; i < sizeof(cube_positions) / sizeof(cube_positions[0]); i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			float angle = i * 20.0f;
			model = glm::translate(model, cube_positions[i]);
			model = glm::rotate(model, glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));  

			int model_loc = glGetUniformLocation(shader.ID, "model");
			glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// because GLFW uses double-buffering by default, we need to swap buffers
		// it is also necessary to process OS window events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// de-allocate resources
	shader.Delete();
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow* window, float dt)
{
	const float camera_speed = 2.0f * dt;
	struct key_action {
		int key;
		float fov_change;
		glm::vec3 movement;
	};

	key_action actions[] = {
		{ GLFW_KEY_UP,    10.0f      , glm::vec3(0.0f) },
		{ GLFW_KEY_DOWN, -10.0f      , glm::vec3(0.0f) },
		{ GLFW_KEY_R,     45.0f - fov, glm::vec3(0.0f) },
		{ GLFW_KEY_W,     0.0f       , camera_speed * camera_front },
		{ GLFW_KEY_S,     0.0f       , -camera_speed * camera_front },
		{ GLFW_KEY_A,     0.0f       , -glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed },
		{ GLFW_KEY_D,     0.0f       , glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed }
	};

	for (int i = 0; i < sizeof(actions) / sizeof(actions[0]); i++)
	{
		if (i <= 2)
		{
			if (glfwGetKey(window, actions[i].key) == GLFW_PRESS && (!key_state[actions[i].key]))
			{
				fov += actions[i].fov_change;
				key_state[actions[i].key] = true;
			}
		}
		else
		{
			if (glfwGetKey(window, actions[i].key) == GLFW_PRESS)
				camera_pos += actions[i].movement;
		}

		if (glfwGetKey(window, actions[i].key) == GLFW_RELEASE)
			key_state[actions[i].key] = false;
	}
}
