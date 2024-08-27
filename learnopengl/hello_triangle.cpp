#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void FramebufferResize(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char* vertex_shader_source = "#version 330 core\n"
	"layout (location = 0) in vec3 a_pos;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = vec4(a_pos.x, a_pos.y, a_pos.z, 1.0f);\n"
	"}\0";

const char* fragment_shader_source_red = "#version 330 core\n"
	"out vec4 frag_color;\n"
	"void main()\n"
	"{\n"
	"	frag_color = vec4(1.0f, 0.2f, 0.2f, 1.0f);\n"
	"}\0";

const char* fragment_shader_source_yellow = "#version 330 core\n"
	"out vec4 frag_color;\n"
	"void main()\n"
	"{\n"
	"	frag_color = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
	"}\0";

int main()
{
	// initializing glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (!window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, FramebufferResize);

	// initializing GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// build and compile shader program
	// --------------------------------
	// vertex shader
	unsigned int vertex_shader;
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
	glCompileShader(vertex_shader);

	// fragment shader(s)
	unsigned int fragment_shader_red;
	fragment_shader_red = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader_red, 1, &fragment_shader_source_red, NULL);
	glCompileShader(fragment_shader_red);

	unsigned int fragment_shader_yellow;
	fragment_shader_yellow = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader_yellow, 1, &fragment_shader_source_yellow, NULL);
	glCompileShader(fragment_shader_yellow);

	// link shaders
	unsigned int shader_program_red = glCreateProgram();
	glAttachShader(shader_program_red, vertex_shader);
	glAttachShader(shader_program_red, fragment_shader_red);
	glLinkProgram(shader_program_red);

	unsigned int shader_program_yellow = glCreateProgram();
	glAttachShader(shader_program_yellow, vertex_shader);
	glAttachShader(shader_program_yellow, fragment_shader_yellow);
	glLinkProgram(shader_program_yellow);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader_red);
	glDeleteShader(fragment_shader_yellow);

	// set up vertex data, buffers and configure vertex attributes
	// -----------------------------------------------------------
	float first_triangle_vertices[9] = {
		-0.8f,  -0.8f, 0.0f,  // left
		-0.3f,  -0.8f, 0.0f,  // right
		-0.55f, -0.3f, 0.0f,  // top
	};

	float second_triangle_vertices[18] = {
		-0.3f,  -0.8f, 0.0f,  // left
		 0.0f,  -0.8f, 0.0f,  // right
		-0.15f, -0.3f, 0.0f,  // top
	};

	unsigned int VBO1, VAO1;
	glGenVertexArrays(1, &VAO1);
	glGenBuffers(1, &VBO1);

	glBindVertexArray(VAO1);

	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(first_triangle_vertices), first_triangle_vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	// unbind VBO and VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	unsigned int VBO2, VAO2;
	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);

	glBindVertexArray(VAO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(second_triangle_vertices), second_triangle_vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	// unbind VBO and VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// uncomment for wireframe mode
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// input
		ProcessInput(window);

		// rendering
		glUseProgram(shader_program_red);
		glBindVertexArray(VAO1);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glUseProgram(shader_program_yellow);
		glBindVertexArray(VAO2);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// de-allocate all resources
	glDeleteVertexArrays(1, &VAO1);
	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO1);
	glDeleteBuffers(1, &VBO2);
	glDeleteProgram(shader_program_red);
	glDeleteProgram(shader_program_yellow);

	glfwTerminate();
	return 0;
}

void FramebufferResize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
