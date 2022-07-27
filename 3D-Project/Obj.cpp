#include "Obj.h"
#include "Shader.h"


Obj::Obj()
	: rotation(0.0f),
	  pos(glm::vec3(0.0f,0.0f,-4.0f))
{
	
	// Creating Vertex buffer
	glGenBuffers(1, &this->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 8 * 3 * sizeof(float), VERTICES, GL_STATIC_DRAW);
	this->AttachBufferData();

	// Creating Index Buffer
	glGenBuffers(1, &this->ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INDICES), INDICES, GL_STATIC_DRAW);


};


void Obj::update(Shader& shader)
{
	this->AttachBufferData();
	
	this->rotation += .7;
    
	

	// Just rotating model
	model_matrix = glm::translate(glm::mat4(1), this->pos);
	model_matrix = glm::rotate(model_matrix, glm::radians(rotation), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(shader.u_modelMatrix, 1, false, glm::value_ptr(model_matrix));
}



/* As in this game I'll be using one VAO, need to keep re-attaching content into array for 
   obj e.g. Vertices, Normals, Tex-Coords etc.*/ 
void Obj::AttachBufferData()
{

	this->Bind();
	
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, VERTICE_SIZE, GL_FLOAT, GL_FALSE, VERTICE_SIZE * sizeof(float), 0);

}



void Obj::Bind() 
{

	// Decide & change to either have many VAO's or 1 VAO & keep changing the data at position 0 in predominant VAO

	//glBindVertexArray(this->vao);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);
}