#include "Renderer.h"

Renderer::Renderer(uint32_t program)
	:vertexArrayObject(NULL),
	vertexBufferObject(NULL),
	indexBufferObject(NULL),
	program(program)
{
}

Renderer::~Renderer()
{
}

void Renderer::create(float* vertices, size_t vertexSize, uint32_t* indices, size_t indexSize, uint32_t indexCount)
{
	verticesSize = vertexSize;
	indicesCount = indexCount;

	/*Creating and Binding Vertex Array Object*/
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	/*Creating and Binding Vertex Buffer Object*/
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, vertexSize, nullptr, GL_DYNAMIC_DRAW);

	/*Enable and Setup Vertex Attribute Pointer*/
	//for position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(0);

	//for color
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	/*creating and binding Index Buffer Object*/
	glGenBuffers(1, &indexBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize, indices, GL_STATIC_DRAW);


	/*unbinding VAO,VBO, and IBO*/
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//getting uniform location
	//uModel = glGetUniformLocation(program, "model");
	//uColor = glGetUniformLocation(program, "color");
	
}

void Renderer::update(glm::mat4* modelMatrix, glm::vec4* color)
{
	//glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(*modelMatrix));
	//glUniform4fv(uColor, 1, glm::value_ptr(*color));
}


void Renderer::render(float* vertices)
{
	glBindVertexArray(vertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
	glBufferSubData(GL_ARRAY_BUFFER, 0, verticesSize, vertices);

	glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT,0);
}
