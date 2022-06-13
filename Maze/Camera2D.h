#pragma once

#include<GL/glew.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
class Camera2D
{
	private:
		uint32_t uView;
		uint32_t uProjection;
		uint32_t program;
		
	public:
		Camera2D(uint32_t shaderProgram);

		void update(glm::mat4* viewMatrix, glm::mat4* projectionMatrix);
		
};

