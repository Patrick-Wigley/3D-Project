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

const unsigned int NUM_BUFFERS = 6U;
const unsigned int NUM_OF_VBOS = 5U;
const enum class BUFFER_TYPE {
	VERTEX=0, 
	UV=1,
	NORMAL=2,
	BONES=3,
	
	INDEX=5, 

	VBO_COUNT=4,
	BUFFERS_COUNT=5
};
// BUFFER_TYPES are also organised in VAO buffer locations order
typedef BUFFER_TYPE BUFFER_LOCATION;

const unsigned int MAX_BONES_PER_VERTEX = 4;

/* Instance will hold influential BONE's & WEIGHTS data for a vertex */
struct VertexBoneData
{
	unsigned int BoneIDs[MAX_BONES_PER_VERTEX]{ 0 };
	float Weights[MAX_BONES_PER_VERTEX]{ .0f };
	
	void AddData(unsigned int BoneID, float weight)
	{
		for (unsigned int i = 0; i < ARRAY_COUNT(BoneIDs, unsigned int); i++)
		{
			if (Weights[i] == .0)
			{
				this->BoneIDs[i] = BoneID;
				this->Weights[i] = weight;
				return;
			}
		}

		assert(false);
		printf("ERROR LOADING BONES");
	}
};


struct MeshWorldTranslation
{
private:
	glm::vec3 m_Pos;
	float m_Rotation;
	glm::mat4 m_ModelMatrix;

public:
	void UpdateMatrix(glm::vec3 pos, float rotation);

};

struct MeshEntry
{
	MeshEntry()
		: NumIndices(0), BaseVertex(0), BaseIndex(0), MaterialIndex(NULL)
	{}
	unsigned int NumIndices;
	unsigned int BaseVertex;
	unsigned int BaseIndex;
	unsigned int MaterialIndex;

	MeshWorldTranslation mat;
};

// #-#-#-#- ASSIMP MODELS -#-#-#-#
// Beginning changeover to full assimp usage - Delete when complete
// Assimp usage
class Model
{	// Each model will now have it's own VAO. A mesh will contain its own buffer data which will be attached to its models VAO when drawing.
public:
	/* Load a Models Data into DataStructure
	- THIS SHOULD BE CALLED DURING PROGRAM SETUP */
	void LoadModel(const std::string& fileName, float scale);

public:
	unsigned int m_VerticesCount;
	unsigned int m_IndicesCount;
	unsigned int m_MeshesCount;
	unsigned int m_TextureCount;
	unsigned int m_BonesCount;

private:
	bool InitialiseMeshesFromScene(const aiScene* pScene);
	void SetCounts(const aiScene*);
	
	void ExtractMeshesData(const aiScene* pScene);
	void ReserveArrays();
	void InitialiseSingleMesh(const aiMesh* paiMesh);
	void ExtractMeshBones(const aiMesh* paiMesh);
	void InitialiseSingleMeshBone(const aiBone* pBone);
	void ExtractMaterialData(const aiScene* pScene);
	
	void SetupBuffers();


public:
	void AttachModelsVAO();
	void AttachMaterialsTextures(unsigned int location);

	// Currently DataStructure Model holds its own VAO & buffers data in array "m_Buffers".
private:
	//buffers ModelMeshes[] ??
	unsigned int m_VAO;
	GLuint m_Buffers[NUM_BUFFERS];
	unsigned int* m_Textures;
	unsigned int m_MaterialsCount;

	// Arrays
private:
	//std::vector<aiMesh> m_aiMeshes;
	std::vector<MeshEntry> m_Meshes;

	std::vector<Vector3> m_Vertices;
	std::vector<Vector3> m_Normals;
	std::vector<Vector2> m_UVs;
	std::vector<unsigned int> m_Indices;
	std::vector<VertexBoneData> m_Bones;

	

	// Model Modifiers
private:
	float m_ModelScale;


	// Getters
public:
	/*
	MODELS MESH DATA
	  Mesh data may contain the following:
		- Base Index & Vertex
		- Material Index
		- Indicies Count */
	MeshEntry& GetMeshIndex(unsigned int index) { return this->m_Meshes[index]; };
	glm::vec3 GetModelScaleMatrix() { return glm::vec3(this->m_ModelScale, this->m_ModelScale, this->m_ModelScale);  }


private:
	std::string m_ObjFileName;


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

