#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <cmath>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void checkShader(unsigned int shader);
void checkShaderProgram(unsigned int shaderProgram);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// NDC vertices
float vertices[] = {
	0.5f, 0.0f, 0.0f,
	0.0f, -0.5f, 0.0f,
	0.0f, 0.5f, 0.0f,
	-0.5f, 0.0f, 0.0f,
};
// for EBO
unsigned int indices[] = {
    0, 1, 3,
	1, 2, 3,
	0, 2, 3,
};

const char* vertexShaderSource = ""
	"#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main() {\n"
	"	gl_Position = vec4(aPos, 1.0f);\n"
	"}\0";
	
const char* fragmentShaderSource = ""
	"#version 330 core\n"
    "out vec4 fragColor;\n"
	"uniform vec4 vertexColor;"
    "void main() {\n"
    "	fragColor = vertexColor;\n"
    "}\0";


int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	
	// start process for drawing
	// vertex shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);
	checkShader(vertexShader);

	// fragment shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);
	checkShader(fragmentShader);

	// shader program
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	checkShaderProgram(shaderProgram);
	// delete shaders already in use
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// vertex array object
	// see images/vertex_array_object.png
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	// vertex buffer object
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// element buffer object
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	/*
		Params
		1) which vertex attribute configure (location in vertex shader)
		2) The number of componentsper vertex
		3) the data type of the vertices
		4) If we want normalized data, which is used for integers
		5) the stride, the space between each vertex (in this case, it is 12 bytes)
		6) The offset of where the position data begins
	*/
	// see images/vertex_buffer.png
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		processInput(window);
		
		// clear background
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);

		// update uniform
		float time = glfwGetTime();
		float green = (sin(time) / 2.0f) + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "vertexColor");
		glUniform4f(vertexColorLocation, 0.0f, green, 0.0f, 1.0f);
		// send data to vertex array
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void checkShader(unsigned int shader) {
	static int success;
	static char log[512];

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	glGetShaderInfoLog(shader, 512, NULL, log);
	std::cout << "Shader log: " << log << "\n";
}

void checkShaderProgram(unsigned int shaderProgram) {
	static int success;
	static char log[512];

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	glGetProgramInfoLog(shaderProgram, 512, NULL, log);
	std::cout << "Program log: " << log << "\n";
}
