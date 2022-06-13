#include "Camera2D.h"
Camera2D::Camera2D(uint32_t shaderProgram)
	:program(shaderProgram)
{
	//getting uniform location
	uProjection = glGetUniformLocation(program, "projection");
	uView = glGetUniformLocation(program, "view");
}


void Camera2D::update(glm::mat4* viewMatrix, glm::mat4* projectionMatrix)
{
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, glm::value_ptr(*projectionMatrix));
	glUniformMatrix4fv(uView, 1, GL_FALSE, glm::value_ptr(*viewMatrix));
}
