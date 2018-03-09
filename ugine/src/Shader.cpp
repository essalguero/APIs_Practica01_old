#include "Shader.h"

#include <string>

Shader::Shader()
{
}


Shader::~Shader()
{
	if (id != -1)		glDeleteProgram(id);
}

// Gets as parameter the code of the shaders to compile
Shader::Shader(string vertexShaderSource, string fragmentShaderSource) {
	
	int retCode = GL_FALSE;
	char errorLog[1024];

	// create vertex shader
	uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char* vertexShaderC = vertexShaderSource.c_str();
	glShaderSource(vertexShader, 1, &vertexShaderC, nullptr);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &retCode);
	if (retCode == GL_FALSE) {
		glGetShaderInfoLog(vertexShader, sizeof(errorLog), nullptr, errorLog);
		std::cout << errorLog << std::endl;

		error = errorLog;

		return;
	}

	// create fragment shader
	uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fragmentShaderC = fragmentShaderSource.c_str();
	glShaderSource(fragmentShader, 1, &fragmentShaderC, nullptr);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &retCode);
	if (retCode == GL_FALSE) {
		glGetShaderInfoLog(fragmentShader, sizeof(errorLog), nullptr, errorLog);
		std::cout << errorLog << std::endl;

		error = errorLog;

		// Delete the vertex shader already created
		glDeleteShader(vertexShader);

		return;
	}

	// create program
	id = glCreateProgram();
	glAttachShader(id, vertexShader);
	glAttachShader(id, fragmentShader);
	glLinkProgram(id);
	glGetProgramiv(id, GL_LINK_STATUS, &retCode);
	if (retCode == GL_FALSE) {
		glGetProgramInfoLog(id, sizeof(errorLog), nullptr, errorLog);
		std::cout << errorLog << std::endl;

		error = errorLog;

		// Delete the shaders already created
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		return;
	}

	errorLog[0] = '\0';

	glDeleteShader(vertexShader);	glDeleteShader(fragmentShader);

	error = errorLog;


	vposLoc = glGetAttribLocation(id, "vpos");
	vcolorLoc = glGetAttribLocation(id, "vcolor");


}


void Shader::setupAttribs() const {
	glEnableVertexAttribArray(vposLoc);
	glEnableVertexAttribArray(vcolorLoc);

	/*
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);
	*/

	glVertexAttribPointer(vposLoc, 3, GL_FLOAT, false, sizeof(float) * 6, 0);
	glVertexAttribPointer(vcolorLoc, 3, GL_FLOAT, false, sizeof(float) * 6, reinterpret_cast<void*>(sizeof(float) * 3));
}




void Shader::setInt(int loc, int val) {

}
void Shader::setFloat(int loc, float val) {

}
void Shader::setVec3(int loc, const glm::vec3& vec) {

}
void Shader::setVec4(int loc, const glm::vec4& vec) {

}
void Shader::setMatrix(int loc, const glm::mat4& matrix) {

}