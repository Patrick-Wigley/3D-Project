#include "Obj.h"


#include <time.h>



Obj::Obj(float inital_pos[3])
	: rotation(0.0f),
	pos(glm::vec3(inital_pos[0], inital_pos[1], inital_pos[2]))
{};


// Rename to "AttachUniformsData"
void Obj::Update(Shader& shader, Camera& camera)
{	
	glUniformMatrix4fv(shader.u_modelMatrix, 1, false, glm::value_ptr(model_matrix));

	unsigned int index = 0;
	// TEXTURES
	for (unsigned int i = 0; i < shader.textures_count; i++)
	{
		glUniform1i(shader.u_samplers[i], i);
		index++;
	}
	// MAPS
	for (unsigned int i = 0; i < shader.maps_count; i++)
	{
		glUniform1i(shader.u_samplers[i+index], i+index);
	}
}


void Obj::Draw(Shader& shader, Camera& camera)
{
	// Currently used for "Model_Own" entities
	this->Update(shader, camera);

	if (!this->use_index_buffer)
		glDrawArrays(GL_TRIANGLES, 0, vertices_count);
	else
		glDrawElements(GL_TRIANGLES, this->indices_count, GL_UNSIGNED_INT, nullptr);
}




/* NEW Model USAGE*/
DynamicObj::DynamicObj(std::vector<float> inital_pos,
	Model* model, bool use_index_buffer)
	:
		Obj(inital_pos.data()), pModel(model)
{
	this->vertices_count = model->m_VerticesCount;
	this->indices_count = model->m_IndicesCount;
	this->use_index_buffer = use_index_buffer;


	this->model_matrix = glm::translate(glm::mat4(1), this->pos);
	this->model_matrix = glm::rotate(model_matrix, glm::radians(rotation), glm::vec3(1.0f, 0.0f, 0.0f));
	this->model_matrix = glm::scale(model_matrix, pModel->GetModelScaleMatrix());
}

/* Params: 
	- Reference to shader
	- Reference to camera
	- CurrentTime since program started
*/
void DynamicObj::SubDraw(Shader& shader, Camera& camera, float CurrentTime)
{
	// Currently attaching vao for each object draw-call - (This will be optimised by grouping entities with same model together so this will only have to be done once per model).
	this->pModel->AttachModelsVAO();
	// This Content will be best in a parent class member once fully changed over to Assimp Model usage
	this->Update(shader, camera);
	
	
	std::vector<Matrix4f> BoneTransforms = this->pModel->GetCurrentBoneTransforms((float)CurrentTime);
	for (unsigned int i = 0; i < BoneTransforms.size(); i++)
	{
		if (i >= MAX_BONES)
			break;
		glUniformMatrix4fv(shader.m_u_Bones[i], 1, GL_TRUE, (const GLfloat*)(BoneTransforms[i]));
	}


	// Draw call for EACH MESH in model
	for (unsigned int i = 0; i < this->pModel->m_MeshesCount; i++)
	{
		MeshEntry& meshData = this->pModel->GetMeshIndex(i);
		this->pModel->AttachMaterialsTextures(meshData.MaterialIndex);
		glDrawElementsBaseVertex(
			GL_TRIANGLES, 
			meshData.NumIndices, 
			GL_UNSIGNED_INT, 
			(void*)(sizeof(unsigned int) * meshData.BaseIndex),
			meshData.BaseVertex
		);	
	}

}


void DynamicObj::WorldSpaceUpdate()
{
	for (unsigned int i = 0; i < this->pModel->m_MeshesCount; i++)
	{
		MeshEntry& meshData = this->pModel->GetMeshIndex(i);
		
	}
}



/* OLD Model_Own USAGE */

// VAO POSITIONS:
// 1 = Vertex Positions
// 2 = Vertex UVs
/* MAKE SURE MAIN VAO IS ACTIVE */
void StaticObj::AttachBufferData()
{
	/* As in this game I'll be using one VAO, need to keep re-attaching content into array for
	   obj e.g. Vertices, Normals, Tex-Coords etc.*/
	
	// Binds Buffers
	
	// Vertex Position
	glBindBuffer(GL_ARRAY_BUFFER, this->p_model->buffers.vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, VERTICE_SIZE, GL_FLOAT, GL_FALSE, VERTICE_SIZE * sizeof(float), 0);
	

	// This will be optimised as some draws will be using the same texture
	//glBindTexture(GL_TEXTURE_2D, this->p_model->texture_content.model_texture);
	
	// Initially unbind previously used texture
	glBindTexture(GL_TEXTURE_2D, 0);
	// Loop throught models texture(s) & Activate index location, Bind texture
	for (unsigned int i = 0; i < this->p_model->texture_content.textures_count; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, this->p_model->texture_content.model_textures[i]);
	}


	// Vertex UV
	glBindBuffer(GL_ARRAY_BUFFER, this->p_model->buffers.tbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);


	// Bind Index Buffer - Currently not using indices
	if (this->use_index_buffer)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->p_model->buffers.ibo);
}


// THIS FUNCTION IS CURRENTLY DUPLICATED FIX IN FUTURE AFTER ASSIMP MODEL LOADING CHANGEOVER
void BulletObj::AttachBufferData()
{
	/* As in this game I'll be using one VAO, need to keep re-attaching content into array for
	   obj e.g. Vertices, Normals, Tex-Coords etc.*/

	   // Binds Buffers

	   // Vertex Position
	glBindBuffer(GL_ARRAY_BUFFER, this->p_model->buffers.vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, VERTICE_SIZE, GL_FLOAT, GL_FALSE, VERTICE_SIZE * sizeof(float), 0);


	// This will be optimised as some draws will be using the same texture
	//glBindTexture(GL_TEXTURE_2D, this->p_model->texture_content.model_texture);

	// Initially unbind previously used texture
	//glBindTexture(GL_TEXTURE_2D, 0);
	// Loop throught models texture(s) & Activate index location, Bind texture
	for (unsigned int i = 0; i < this->p_model->texture_content.textures_count; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		//glBindTexture(GL_TEXTURE_2D, this->p_model->texture_content.model_textures[i]);
	}


	// Vertex UV
	glBindBuffer(GL_ARRAY_BUFFER, this->p_model->buffers.tbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);


	// Bind Index Buffer - Currently not using indices
	if (this->use_index_buffer)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->p_model->buffers.ibo);
}

void StaticObj::SubDraw(Shader& shader, Camera& camera)
{
	this->AttachBufferData();
	this->Draw(shader, camera);
}
// THIS FUNCTION IS CURRENTLY DUPLICATED FIX IN FUTURE AFTER ASSIMP MODEL LOADING CHANGEOVER
void BulletObj::SubDraw(Shader& shader, Camera& camera)
{
	this->AttachBufferData();
	this->Draw(shader, camera);
}


StaticObj::StaticObj(std::vector<float> inital_pos,
	Model_Own* model, bool use_index_buffer)
	: Obj(inital_pos.data()), p_model(model)
{
	

	// old
	std::vector<float>& vertices = (*model).model_content.vertices;
	std::vector<unsigned int>& indices = (*model).model_content.indices;
	// continue here


	indices_count = (*model).vertices_count;
	vertices_count = (*model).indices_count;

	
	model_matrix = glm::translate(glm::mat4(1), this->pos);
	model_matrix = glm::rotate(model_matrix, glm::radians(rotation), glm::vec3(1.0f, 0.0f, 0.0f));

	


	this->use_index_buffer = use_index_buffer;
}



BulletObj::BulletObj(std::vector<float> inital_pos, glm::vec3 path, 
	Model_Own* model)
	: Obj(inital_pos.data()), p_model(model),
	path_vec(path)
{

	// old
	std::vector<float>& vertices = (*model).model_content.vertices;
	std::vector<unsigned int>& indices = (*model).model_content.indices;
	// continue here


	indices_count = (*model).vertices_count;
	vertices_count = (*model).indices_count;
}



void BulletObj::Sub_Update()
{
	this->rotation += .7f;

	// Just rotating model
	model_matrix = glm::translate(glm::mat4(1), this->pos);
	model_matrix = glm::rotate(model_matrix, glm::radians(rotation), glm::vec3(1.0f, 0.0f, 0.0f));
	//model_matrix = glm::rotate(model_matrix, glm::radians(3.2f), glm::vec3(0.0f, 0.0f, 1.0f));


	this->pos[0] += this->path_vec.x / this->MOVEMENT_SPEED;
	this->pos[1] += this->path_vec.y / this->MOVEMENT_SPEED;
	this->pos[2] += this->path_vec.z / this->MOVEMENT_SPEED;
}








//void Obj::Set_Dynamic(bool result){ this->is_dynamic_mesh = result; }

	// Experimenting 
	
	//if (this->is_dynamic_mesh)
	//{
	//	//glm::mat4 test = *camera.GetRotationMatrix;
	//	this->path += .1f;
	//	glm::vec3 camDir = *camera.Get_Rotation;
	//	this->path = -atan2(camDir.y, camDir.x);
	//	
	//	float speed = 100;
	//
	//
	//	this->pos[0] -= cos(camDir.r) / speed;
	//	this->pos[1] -= sin(camDir.r) / speed;
	//	this->pos[2] -= cos(camDir.r) / speed;
	//}

	//glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(float), mesh.vertices.data(), GL_DYNAMIC_DRAW);




//// Array of glm::vec3 (X,Y,Z)
//for (int i = 0; i < mesh.vertices_wrapped.size(); i++)
//{
//	glm::vec3& vertice = mesh.vertices_wrapped[i];


//	srand(time(NULL));
//	float rand_num = (rand() % 10 + 1);
//
//	vertice.z = -.5 + ((rand_num/10));


//	// Updating vertices list to apply to updated buffer
//	int stride_index = 3 * i;
//	mesh.vertices[stride_index + 0] = vertice.x;
//	mesh.vertices[stride_index + 1] = vertice.y;
//	mesh.vertices[stride_index + 2] = vertice.z;
//}