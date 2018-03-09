/*#ifdef _MSC_VER
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif*/

#include "../lib/glew/glew.h"
#include "../lib/glfw3/glfw3.h"
#include "common.h"
#include <array>
#include <fstream>
#include <iostream>
#include <sstream>


#include "Shader.h"

#define FULLSCREEN false

std::array<float, 18> vertices = {
	// coords	// colors
	 0,  1, 0,		1, 0, 0,
	-1, -1, 0,  	0, 1, 0,
	 1, -1, 0,		0, 0, 1
};

std::string readString(const char* filename) {
	std::ifstream f(filename, std::ios_base::binary);
	std::stringstream ss;
	ss << f.rdbuf();
	return ss.str();
}


int init() {
	

	// init glew
	if (glewInit()) {
		std::cout << "could not initialize glew" << std::endl;
		return -1;
	}

	// enable gl states
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);
	return 0;

}



int main(int, char**) {


	if (glfwInit() != GLFW_TRUE) {
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

	if (init())
		return -1;


	Shader shaderProgram = Shader(readString("data/shader.vert"), readString("data/shader.frag"));

	if (shaderProgram.getError() == "")
	{
		cout << shaderProgram.getError() << endl;
		return -1;
	}

	shaderProgram.use();

	// setup vertex vars
	int vposLoc = glGetAttribLocation(shaderProgram.getId(), "vpos");
	int vcolorLoc = glGetAttribLocation(shaderProgram.getId(), "vcolor");
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
	glVertexAttribPointer(vposLoc, 3, GL_FLOAT, false, sizeof(float) * 6, 0);
	glVertexAttribPointer(vcolorLoc, 3, GL_FLOAT, false, sizeof(float) * 6, reinterpret_cast<void*>(sizeof(float) * 3));


	//Bucle principal

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