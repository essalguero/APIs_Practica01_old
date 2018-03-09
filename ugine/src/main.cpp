#ifdef _MSC_VER
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#include "../lib/glew/glew.h"
#include "../lib/glfw3/glfw3.h"
#include "common.h"
#include <array>
#include <fstream>
#include <iostream>
#include <sstream>

#define FULLSCREEN false

std::array<float, 15> vertices = {
	// coords	// colors
	 0,  1,		1, 0, 0,
	-1, -1,		0, 1, 0,
	 1, -1,		0, 0, 1
};

std::string readString(const char* filename) {
	std::ifstream f(filename, std::ios_base::binary);
	std::stringstream ss;
	ss << f.rdbuf();
	return ss.str();
}

int main(int, char**) {
	if ( glfwInit() != GLFW_TRUE ) {
		std::cout << "could not initalize glfw" << std::endl;
		return -1;
	}
	atexit(glfwTerminate);

	// create window
	//glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "U-gine", FULLSCREEN ? glfwGetPrimaryMonitor() : nullptr, nullptr);
	if (!window) {
		std::cout << "could not create glfw window" << std::endl;
		return -1;
	}
	glfwMakeContextCurrent(window);

	// init glew
	if (glewInit()) {
		std::cout << "could not initialize glew" << std::endl;
		return -1;
	}

	// read shaders
	std::string vertexShader = readString("data/shader.vert");
	std::string fragmentShader = readString("data/shader.frag");
	if (vertexShader == "" || fragmentShader == "") {
		std::cout << "could not load shaders" << std::endl;
		return -1;
	}

	// create vertex shader
	int retCode = GL_FALSE;
	char errorLog[1024];
	uint32_t vs = glCreateShader(GL_VERTEX_SHADER);
	const char* vertexShaderC = vertexShader.c_str();
	glShaderSource(vs, 1, &vertexShaderC, nullptr);
	glCompileShader(vs);
	glGetShaderiv(vs, GL_COMPILE_STATUS, &retCode);
	if (retCode == GL_FALSE) {
		glGetShaderInfoLog(vs, sizeof(errorLog), nullptr, errorLog);
		std::cout << errorLog << std::endl;
		return -1;
	}

	// create fragment shader
	uint32_t fs = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fragmentShaderC = fragmentShader.c_str();
	glShaderSource(fs, 1, &fragmentShaderC, nullptr);
	glCompileShader(fs);
	glGetShaderiv(fs, GL_COMPILE_STATUS, &retCode);
	if (retCode == GL_FALSE) {
		glGetShaderInfoLog(fs, sizeof(errorLog), nullptr, errorLog);
		std::cout << errorLog << std::endl;
		return -1;
	}

	// create program
	uint32_t program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &retCode);
	if (retCode == GL_FALSE) {
		glGetProgramInfoLog(program, sizeof(errorLog), nullptr, errorLog);
		std::cout << errorLog << std::endl;
		return -1;
	}

	// use program
	glUseProgram(program);

	// enable gl states
	glEnable(GL_SCISSOR_TEST);

	// setup vertex vars
	int vposLoc = glGetAttribLocation(program, "vpos");
	int vcolorLoc = glGetAttribLocation(program, "vcolor");
	glEnableVertexAttribArray(vposLoc);
	glEnableVertexAttribArray(vcolorLoc);

	// create gl buffer
	uint32_t buffer = 0;
	glGenBuffers(1, &buffer);
	if ( buffer == 0 ) {
		std::cout << "could not create buffer" << std::endl;
		return -1;
	}
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(vposLoc, 2, GL_FLOAT, false, sizeof(float) * 5, 0);
	glVertexAttribPointer(vcolorLoc, 3, GL_FLOAT, false, sizeof(float) * 5, reinterpret_cast<void*>(sizeof(float) * 2));

	float lastTime = static_cast<float>(glfwGetTime());
	while ( !glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE) ) {
		// update delta time
		float newTime = static_cast<float>(glfwGetTime());
		float deltaTime = newTime - lastTime;
		lastTime = newTime;

		// get updated screen size
		int screenWidth, screenHeight;
		glfwGetWindowSize(window, &screenWidth, &screenHeight);

		// report screen size
		std::stringstream ss;
		ss << screenWidth << "x" << screenHeight;
		glfwSetWindowTitle(window, ss.str().c_str());

		// clear screen
		glViewport(0, 0, screenWidth, screenHeight);
		glScissor(0, 0, screenWidth, screenHeight);
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// draw triangle
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// update swap chain & process events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// delete vertex buffer
	glDeleteBuffers(1, &buffer);
	
	return 0;
}