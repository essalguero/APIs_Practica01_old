#include "Buffer.h"


Buffer::Buffer()
{

}

Buffer::~Buffer() {

}

void Buffer::draw(const Shader& shader) const {


	uint32_t buffer = 0;
	glGenBuffers(1, &buffer);
	if (buffer == 0) {
		std::cout << "could not create buffer" << std::endl;
		return; //return -1
	}
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);

	shader.setupAttribs();

	// draw triangle
	glDrawArrays(GL_TRIANGLES, 0, 3);

}
