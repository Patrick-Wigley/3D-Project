#pragma once
#include "GlobalItems.h"
#include "Shader.h"


#include "Meshes.h"


// For now this is just for an instance/object to use containing its own data regarding vertices etc

const static int VERTICE_SIZE = 3;
const static int TEXTURE_COORDINATE_SIZE = 2;



class Obj
{
private:
	// Pointer to a model with populated content
	Model* p_model;

private:
	int indices_count;
	int vertices_count;

private:
	void AttachBufferData();
	void Update(Shader&, Camera&);



protected:
	glm::vec3 pos;
	float rotation;

	glm::mat4 model_matrix;
	float path = .2f;
	
	bool use_index_buffer = false;
	


public:
	Obj(float inital_pos[3], Model* model
	//	const std::vector<float> uvs
	);
	
	void Draw(Shader&, Camera&);
};





// Inherenting from class 'Obj'

/* Any model which is still in a fixed place */
class StaticObj : public Obj
{
public:
	StaticObj(std::vector<float> inital_pos, 
		Model* model, bool use_index_buffer = false);

};


class BulletObj : public Obj
{
public:
	BulletObj(std::vector<float> inital_pos, glm::vec3 path, 
		Model* model);

	// "Sub" prefix stands for method in this sub-class.
	void Sub_Update();


private:
	const float MOVEMENT_SPEED = 1.2f;
	glm::vec3 path_vec;
};




class TerrainObj
{



};