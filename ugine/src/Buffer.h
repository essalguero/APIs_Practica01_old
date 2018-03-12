#pragma once

#include "Shader.h"

#include "Vertex.h"


#include <vector>

using namespace std;
using namespace glm;

class Buffer
{
public:
	Buffer();

	Buffer(const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices) : vertices(vertices), indices(indices) {
		
		glGenBuffers(2, buffer);
		if (buffer[0] == 0 || buffer[1] == 0) {
			std::cout << "could not create buffers" << std::endl;
			return; //return -1
		}

		size_t vsize = vertices.size() * sizeof(Vertex);
		size_t isize = indices.size() * sizeof(uint16_t);

		// Enlazar los buffers de datos autilizar
		glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[1]);

		// Rellenar los buffers de datos. Tienen que estan enlazados previamente
		glBufferData(GL_ARRAY_BUFFER, vsize, vertices.data(), GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, isize, indices.data(), GL_STATIC_DRAW);

		


	};

	~Buffer();

	void draw(const Shader& shader) const;

private:


	std::vector<Vertex> vertices;
	std::vector<uint16_t> indices;

	uint32_t buffer[2]{ 0 };

};