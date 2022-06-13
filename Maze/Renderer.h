#pragma once
#include<iostream>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>
#include "Vertex.h"

class Renderer
{
	private:
		uint32_t vertexArrayObject;
		uint32_t vertexBufferObject;
		uint32_t indexBufferObject;
		size_t verticesSize;
		uint32_t indicesCount;
		uint32_t program;	

	public:
		Renderer(uint32_t program);
		~Renderer();

		void create(float* vertices, size_t vertexSize, uint32_t* indices, size_t indexSize, uint32_t indexCount);
		void update(glm::mat4* modelMatrix, glm::vec4* color);
		uint32_t getSize() const;
		void render(float* vertices);


};

