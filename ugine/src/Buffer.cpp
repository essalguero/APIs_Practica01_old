#include "Buffer.h"






Buffer::Buffer() {

}


Buffer::~Buffer() {
	glDeleteBuffers(2, buffer);
}

void Buffer::draw(const Shader& shader) const {

	/*size_t vsize = vertices.size() * sizeof(Vertex);
	size_t isize = indices.size() * sizeof(uint16_t);
	*/
	// Volver a enlazar los buffers de datos antes de usarlos
	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[1]);
	/*
	// Rellenar los buffers de datos. Tienen que estan enlazados previamente
	glBufferData(GL_ARRAY_BUFFER, vsize, vertices.data(), GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, isize, indices.data(), GL_STATIC_DRAW);
	*/

	shader.setupAttribs();

	// draw triangle
	//glDrawArrays(GL_TRIANGLES, 0, 3);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, nullptr);
}
