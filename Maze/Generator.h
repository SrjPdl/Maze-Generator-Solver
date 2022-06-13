#pragma once
#include<vector>
#include "Vertex.h"
namespace Generator {

	static std::vector<Vertex>generateQuad(uint32_t xPos, uint32_t yPos, glm::vec4 color, uint32_t size)
	{	
		Vertex v0;
		v0.position = glm::vec3(xPos, yPos, 0.f);
		v0.color = color;

		Vertex v1;
		v1.position = glm::vec3(xPos, yPos + size, 0.f);
		v1.color = color;

		Vertex v2;
		v2.position = glm::vec3(xPos + size, yPos + size, 0.f);
		v2.color = color;

		Vertex v3;
		v3.position = glm::vec3(xPos + size, yPos, 0.f);
		v3.color = color;


		return { v0, v1, v2, v3 };
	}
}