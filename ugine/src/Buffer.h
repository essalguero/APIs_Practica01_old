#pragma once

#include "Shader.h"

#include <vector>

using namespace std;
using namespace glm;

class Buffer
{
public:
	Buffer();
	Buffer(vector<vec3> vertices, vector<int> indices) : vertices(vertices), indices(indices) {};
	~Buffer();

	void draw(const Shader& shader) const;

private:
	vector<vec3> vertices;
	vector<int> indices;
};