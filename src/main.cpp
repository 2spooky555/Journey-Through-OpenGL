#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void checkShader(unsigned int shader);
void checkShaderProgram(unsigned int shaderProgram);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float vertices[] = {
	// vertices  		colors             texture coords
	0.5f, 0.5f, 0.0f,	1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
	0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
	-0.5f, -0.5f, 0.0f,	0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
	-0.5f, 0.5f, 0.0f,	1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
};
unsigned int indices[] = {
	0, 1, 3,
	1, 2, 3,
};

const char* vertexShaderSource = ""
	"#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"layout (location = 1) in vec3 aColor;\n"
	"layout (location = 2) in vec2 aTextCoord;\n"
	"out vec3 ourColor;\n"
	"out vec2 textCoord;\n"
	"void main() {\n"
	"	gl_Position = vec4(aPos, 1.0f);\n"
	"	ourColor = aColor;\n"
	"	textCoord = aTextCoord;\n"
	"}\0";
	
const char* fragmentShaderSource = ""
	"#version 330 core\n"
    "out vec4 fragColor;\n"
	"in vec3 ourColor;\n"
	"in vec2 textCoord;\n"
	"uniform sampler2D texture1;\n"
    "void main() {\n"
    "	fragColor = texture(texture1, textCoord);\n"
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
	// element buffer object
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// loading a texture
	stbi_set_flip_vertically_on_load(true);
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width;
	int height;
	int nrChannels;
	unsigned char* data = stbi_load("../images/face.png", &width, &height, &nrChannels, 0);
	if (!data) {
		std::cout << "Failed to load image!\n";
	}
	/*
		Parameters:
		1) Specify texture setting
		2) Specify minimap level if you want to make them manually
		3) Color image format
		4) texture width
		5) texture height
		6) Always be 0!(legacy reasons)
		7) Color format
		8) Datatype of source image
		9) image data
	*/
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		processInput(window);
		
		// clear background
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// bind texture, then use shader
		glBindTexture(GL_TEXTURE_2D, texture);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
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
