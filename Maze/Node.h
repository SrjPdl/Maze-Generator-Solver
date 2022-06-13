#pragma once
#include "glm/glm.hpp"

enum {
	none,
	up,
	down,
	left,
	right,
};

struct Node
{

	glm::vec<2, int> location;
	
	Node() :location(glm::vec<2, int>(0, 0)) {};
	Node(uint32_t x, uint32_t y) :location(glm::vec<2, int>(x, y)) {};
};

