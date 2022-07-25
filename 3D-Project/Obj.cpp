#include "Obj.h"



Obj::Obj()
{
	// Creating Vertex Array Object & Binding
	glGenVertexArrays(1, &this->vao);
	glBindVertexArray(vao);

	// Creating Vertex buffer to bind to vao
	glGenBuffers(1, &this->vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	this->AttachBufferData();

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);


	glGenBuffers(1, &this->ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), INDICES, GL_STATIC_DRAW);


};


void Obj::AttachBufferData()
{

	glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(float), VERTICES, GL_STATIC_DRAW);
}

void Obj::Bind() 
{

	// Decide & change to either have many VAO's or 1 VAO & keep changing the data at position 0 in predominant VAO

	glBindVertexArray(this->vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);
}