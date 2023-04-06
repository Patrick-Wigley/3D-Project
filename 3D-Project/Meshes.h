#pragma once
#include <vector>
#include "GlobalItems.h"

// For loading images

#include "stb_image.h"

// Prefix to texture files
const static std::string TEXTURE_FOLDER = "3D-Project\\Assets\\Textures\\";
const static std::string MAPS_FOLDER = "3D-Project\\Assets\\Maps\\";

// #-#-#-#- MODELS -#-#-#-#

// Content for a model
struct RawModelContent
{
	std::vector<float>		  vertices;
	std::vector<float>		  texture_Coordinates;
	std::vector<float>		  normals;
	std::vector<unsigned int> indices;


};
// Instance will contain a buffer for a specific 
struct RawTexture
{	
	// Textures ID
	unsigned int model_texture = NULL;
	
	unsigned int* model_textures;
	unsigned int textures_count;

	std::vector<std::string> file_names;
	std::string file_name;
};



struct Buffers
{
	unsigned int vbo;
	unsigned int ibo;
	unsigned int tbo;
	
	unsigned int texture_id;
};

/* PARENT CLASS FOR MODELS */
class Model
{

	// Protected Fields
public:
	// Where Vertices, Indices, UV's, Normals are stored
	RawModelContent model_content;
	RawTexture texture_content;
	Buffers buffers;


	int vertices_count;
	int indices_count;

	~Model()
	{
		std::cout << "\n" << "[Object]: Deleting Model";
	};
	void SetUp(std::string FileName);

protected:
	/* Set Vertices & Indices count */
	void Set_Counts(RawModelContent& content)
	{
		this->vertices_count = content.vertices.size();
		this->indices_count  = content.indices.size();
	}
	
};



// Models - SHOULD BE LOADED IN ONCE AT START


// This Model_Global class is intend to be used for different models. Rather than having a unique class for each model
class Model_Global : public Model
{
public:
	Model_Global();
	// Getters - (References)
	RawModelContent& GetModelContent = this->model_content;
	RawTexture& GetTextureContent = this->texture_content;
};





// Will contain the THREE lists (Vertex Position, uv, normal)
// lists to be populated from prevailing line.
struct OBJ_Face_3D
{
	std::vector<unsigned int> vertex_pos;
	std::vector<unsigned int> vertex_uv;
	std::vector<unsigned int> vertex_norm;
};



class Terrain : public Model
{
public:
	Terrain();
	void SetUp();

	// Getters - (References)
	RawModelContent& GetModelContent = this->model_content;
	RawTexture& GetTextureContent = this->texture_content;
	
	void ActivateShader(bool activate);
	//Shader shader = Shader(SHADER_FOLDER_DIR + "Terrain.Shader");
	


	float find_positions_height(float* camera_pos);
private:
	std::vector<glm::vec3> vertice_values;
	float* vertices_size = (float*)malloc(sizeof(float)*2);
};





/* Store heightmap's, blendmap's data while for applying to mesh */ 
struct Maps
{
	unsigned char* data = nullptr;
	int width, height, bit_per_pixel;
};;
const int MAX_RGB_VAL = (255 * 3);


// Create terrain
Model GenTerrain(Maps*, std::vector<glm::vec3> & y_values);
void setup_buffers(Buffers& buffers, RawModelContent& model_content, RawTexture& texture_content);

