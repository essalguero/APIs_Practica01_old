#include "common.h"
#include "Shader.h"
#include "Vertex.h"

#include <string>

Shader::Shader()
{
}


Shader::~Shader()
{
	if (id != -1)
		glDeleteProgram(id);

}

// Gets as parameter the code of the shaders to compile
Shader::Shader(string vertexShaderSource, string fragmentShaderSource) : error{} {
	
	int retCode = GL_FALSE;

	// create vertex shader
	uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char* vertexShaderC = vertexShaderSource.c_str();
	glShaderSource(vertexShader, 1, &vertexShaderC, nullptr);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &retCode);
	if (retCode == GL_FALSE) {
		glGetShaderInfoLog(vertexShader, sizeof(error), nullptr, error);
		std::cout << error << std::endl;

		return;
	}

	// create fragment shader
	uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fragmentShaderC = fragmentShaderSource.c_str();
	glShaderSource(fragmentShader, 1, &fragmentShaderC, nullptr);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &retCode);
	if (retCode == GL_FALSE) {
		glGetShaderInfoLog(fragmentShader, sizeof(error), nullptr, error);
		std::cout << error << std::endl;

		// Delete the vertex shader already created in case of error
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
		glGetProgramInfoLog(id, sizeof(error), nullptr, error);
		std::cout << error << std::endl;

		// Delete the shaders already created in case of error
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		return;
	}

	// Delete the shaders once the program has been created
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	vposLoc = glGetAttribLocation(id, "vpos");
	vcolorLoc = glGetAttribLocation(id, "vcolor");

	mvpMatrix = glGetUniformLocation(id, "mvpMatrix");

}


void Shader::setupAttribs() const {
	if (vposLoc != -1) 
		glEnableVertexAttribArray(vposLoc);

	if (vcolorLoc != -1)
		glEnableVertexAttribArray(vcolorLoc);


	glVertexAttribPointer(vposLoc, 3, GL_FLOAT, false, sizeof(Vertex), 0);
	//glVertexAttribPointer(vcolorLoc, 3, GL_FLOAT, false, sizeof(float) * 6, reinterpret_cast<void*>(sizeof(float) * 3));
}




void Shader::setInt(int loc, int val) {
	if (loc != -1)
	{
		glUniform1i(loc, val);
	}
}
void Shader::setFloat(int loc, float val) {
	if (loc != -1)
	{
		glUniform1f(loc, val);
	}

}
void Shader::setVec3(int loc, const glm::vec3& vec) {
	if (loc != -1)
	{
		glUniform3f(loc, vec.x, vec.y, vec.z);
	}

}
void Shader::setVec4(int loc, const glm::vec4& vec) {
	if (loc != -1)
	{
		glUniform4f(loc, vec.r, vec.g, vec.b, vec.a);
	}

}
void Shader::setMatrix(int loc, const glm::mat4& matrix) {
	if (loc != -1)
	{
		glUniformMatrix4fv(loc, 1, false, glm::value_ptr(matrix));
	}

}


int	Shader::getLocation(const char* name) const {

	int location = glGetUniformLocation(id, name);

	return location;
}