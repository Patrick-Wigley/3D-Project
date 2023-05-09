#pragma once
#include "GlobalItems.h"

#include "Shader.h"
#include "Models.h"


// For now this is just for an instance/object to use containing its own data regarding vertices etc

const static int VERTICE_SIZE = 3;
const static int TEXTURE_COORDINATE_SIZE = 2;





class Obj
{
private:
	//Model* pAssimpModel;


protected:
	int indices_count;
	int vertices_count;

protected:
	bool ModelRigged;

protected:
	void Update(Shader&, Camera&);



protected:
	glm::vec3 pos;
	float rotation;

	glm::mat4 model_matrix;
	float path = .2f;
	
	bool use_index_buffer = false;
	


public:
	Obj(float inital_pos[3]//, Model_Own* model, Model* AssimpModel=nullptr
	//	const std::vector<float> uvs
	);
	
	void Draw(Shader&, Camera&);
};




// Inherenting from class 'Obj'
// Objects Utilising Assimp Models - (StaticObj will need to be updated to use assimp models in future) - (Currently in process or Parallel-Changeover)
class DynamicObj : public Obj
{
private:
	/* Pointer to a loaded model data-structure */
	Model* pModel;

public:
	// For now, this is here as other shaders do not have this uniform
	// Should make a shader class which is specific for the main.glsl uniforms
	// Uniform Location for bone transformations
	

public:
	/* Entities Draw calls for its designated model meshes */
	void SubDraw(Shader& shader, Camera& camera, float CurrentTime);
	/* 
	Update Entities world-space:
	    WORLD SPACE TRANSFORMATIONS	*/
	void WorldSpaceUpdate();

public:
	DynamicObj(std::vector<float> inital_pos,
		Model* model, bool use_index_buffer = false);


};


/* Any model which is still in a fixed place */
class StaticObj : public Obj
{
private:
	/* Pointer to a model with populated content - THIS IS NOT AN ASSIMP LOADED MODEL */ 
	Model_Own* p_model;

public:
	void AttachBufferData();
	void SubDraw(Shader& shader, Camera& camera);

public:
	StaticObj(std::vector<float> inital_pos, 
		Model_Own* model, bool use_index_buffer = false);
};


class BulletObj : public Obj
{
private:
	// Pointer to a model with populated content
	Model_Own* p_model;

public:
	void AttachBufferData();
	void SubDraw(Shader& shader, Camera& camera);

public:
	BulletObj(std::vector<float> inital_pos, glm::vec3 path, 
		Model_Own* model);

	// "Sub" prefix stands for method in this sub-class.
	void Sub_Update();


private:
	const float MOVEMENT_SPEED = 1.2f;
	glm::vec3 path_vec;
};




class TerrainObj
{



};