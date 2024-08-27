#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Shader.h"

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Shaders", NULL, NULL);
	if (!window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD";
		return -1;
	}

	// initializing shaders with shader class
	Shader shader("./vertex.glsl", "./fragment.glsl");

	float vertices[18] = {
		// positions          //colors
		-0.5f,  -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // left
		 0.5f,  -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // right
		-0.0f,   0.5f, 0.0f,  0.0f, 0.0f, 1.0f,   // top
	};

	// vbo, vao
	unsigned int VBO, VAO;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	bool forward = true;
	float horizontal_offset = 0.0f;
	while (!glfwWindowShouldClose(window))
	{
		// input
		// TODO: add input processing

		// rendering
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.Use();
		
		if (horizontal_offset + 0.5f >= 1.0f)
			forward = false;
		else if (horizontal_offset - 0.5f <= -1.0f)
			forward = true;

		forward ? horizontal_offset += 0.001f : horizontal_offset -= 0.001f;
		
		shader.SetFloat("offset", horizontal_offset);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// switching buffers and processing events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// de-allocate all resources
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	shader.Delete();

	glfwTerminate();
	return 0;
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}