#pragma once
#include <vector>
#include "GlobalItems.h"

// For loading images

#include "stb_image.h"

/* Constants */
// Prefix to 'Assets/Textures'
const static std::string TEXTURE_FOLDER = "3D-Project\\Assets\\Textures\\";
// Prefix to 'Assets/Maps'
const static std::string MAPS_FOLDER = "3D-Project\\Assets\\Maps\\";

const unsigned int NUM_BUFFERS = 4U;
const enum class BUFFER_TYPE {
	VERTEX=0, 
	UV=1,
	NORMAL=2,
	INDEX=3, 
};
// BUFFER_TYPES are also organised in VAO buffer locations order
typedef BUFFER_TYPE BUFFER_LOCATION;


struct MeshEntry
{
	MeshEntry()
		: NumIndices(0), BaseVertex(0), BaseIndex(0), MaterialIndex(NULL)
	{}
	unsigned int NumIndices;
	unsigned int BaseVertex;
	unsigned int BaseIndex;
	unsigned int MaterialIndex;
};

// #-#-#-#- ASSIMP MODELS -#-#-#-#
// Beginning changeover to full assimp usage - Delete when complete
// Assimp usage
class Model
{	// Each model will now have it's own VAO. A mesh will contain its own buffer data which will be attached to its models VAO when drawing.
public:
	void LoadModel(const std::string& fileName);

public:
	unsigned int m_VerticesCount;
	unsigned int m_IndicesCount;

private:
	bool InitialiseMeshesFromScene(const aiScene* pScene);
	void SetCounts(const aiScene*);
	
	void ExtractMeshesData(const aiScene* pScene);
	void ReserveArrays();
	void InitialiseSingleMesh(const aiMesh* paiMesh);
	void ExtractMaterialData(const aiScene* pScene);
	void SetupBuffers();


private:
	//buffers ModelMeshes[] ??
	unsigned int m_VAO;
	GLuint m_Buffers[NUM_BUFFERS];
	unsigned int* m_Textures;


	// Arrays
private:
	//std::vector<aiMesh> m_aiMeshes;
	std::vector<MeshEntry> m_Meshes;

	std::vector<Vector3> m_Vertices;
	std::vector<Vector3> m_Normals;
	std::vector<Vector2> m_UVs;
	std::vector<unsigned int> m_Indices;


private:
	std::string m_ObjFileName;


	// Counts
private:
	


	// Constructors
public:
	Model();
};



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
// Previously used Model Class
class Model_Own
{

	// Protected Fields
public:
	// Where Vertices, Indices, UV's, Normals are stored
	RawModelContent model_content;
	RawTexture texture_content;
	Buffers buffers;


	int vertices_count;
	int indices_count;

	~Model_Own()
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
class Model_Global : public Model_Own
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



class Terrain : public Model_Own
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
Model_Own GenTerrain(Maps*, std::vector<glm::vec3> & y_values);
void setup_buffers(Buffers& buffers, RawModelContent& model_content, RawTexture& texture_content);

