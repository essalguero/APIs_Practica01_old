#pragma once

#include "../lib/glm/vec3.hpp"


class Vertex
{
public:
	inline Vertex() : position(0, 0, 0) {};

	Vertex(glm::vec3 pos) : position(pos) {};

	//~Vertex();


	glm::vec3 position;

};

