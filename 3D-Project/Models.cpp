#include "Models.h"


// Vars utilised globally in heap
static unsigned int TotalLoadedTextures = 0;



const enum class FILE_TYPES
{
	OBJ = 1, X = 2, MTL = 3
};



// Count reccurance of spaces in string
unsigned int str_find_val(std::string string, const char* delimiter)
{
	unsigned int count = 1;
	for (unsigned int i = 0; (i = string.find(delimiter, i)) != std::string::npos; i++)
	{
		count++;
	}
	return count;
}


// Reading OBJ
// Splits & Returns contents from prevailing line - (Vertices, Texture-coords & Normals)
std::vector<float> OBJ_append_content(std::string line)
{

	std::string line_ = line;
	std::vector<float> list;


	// Will always be atleast 1 point. If finds separation " " than increments count 
	unsigned int point_count = str_find_val(line, " ");

	std::string point;
	for (unsigned int i = 0; i < point_count; i++)
	{
		line = line_;
		int space_pos = line.find(" ");

		if (space_pos != -1)
			point = line.erase(line.find(" "));
		else
			point = line;


		// Cast value to float & push back into list
		list.push_back(std::stof(point));



		line_.erase(0, space_pos + 1);
	}

	return list;

	//list.push_back();

};
OBJ_Face_3D OBJ_append_indices(std::string line)
{
	line += " ";
	unsigned int faces_on_line = str_find_val(line, " ");

	std::vector<std::string> faces;

	OBJ_Face_3D face_content;



	unsigned int pos = 0;
	// One face/line will contain THREE vertices. each vertice data will consist of: vert_pos /  vert_uv / vert_norm
	while ((pos = line.find(" ")) != std::string::npos)
	{
		// I added "/" to the end to fix issue with not collecting final value in face
		std::string face = line.substr(0, pos) + "/";

		// Taking first value in face - (vertex position)
		std::string val = face.substr(0, face.find("/"));



		// Iterate through vertex pos, uv, norm
		int data_position_counter = 0;

		// 3 Points for Triangle 
		unsigned int i;
		while ((i = face.find("/")) != std::string::npos)
		{

			std::string val = face.substr(0, i);

			// Blender inserts values starting at 1
			unsigned int data = std::stoi(val) - 1;

			if (data_position_counter == 0)
				face_content.vertex_pos.push_back(data);

			else if (data_position_counter == 1)
				face_content.vertex_uv.push_back(data);

			else
				face_content.vertex_norm.push_back(data);



			data_position_counter++;
			face.erase(0, i + 1);
		}


		line.erase(0, pos + 1);
	}


	return face_content;

}


/* Reads an OBJ & extract contents */
RawModelContent read_OBJ(std::string file_name)
{


	// NOTE: READ VERTICES, TEX, NORMALS TO LIST OF GLM::VEC3 - ( VEC2 FOR TEX_COORDS) THAN PUT IN ORDER WITH INDICES


	// Will look for: vertices, normals, uvs, indices
	std::ifstream file(file_name);
	std::string current_line;

	RawModelContent model_content;


	std::vector<glm::vec3>		vertex_positions;
	std::vector<glm::vec2>		vertex_tex_coords;
	std::vector<glm::vec3>		vertex_normals;
	std::vector<unsigned int>	indices;


	std::string line;
	while (std::getline(file, current_line))
	{
		// EXTRACTING DATA FROM FILE

		//std::cout << current_line << std::endl;
		line = current_line;

		unsigned int first_space = current_line.find(" ");
		auto line_prefix = current_line.erase(first_space);
		line.erase(0, first_space + 1);


		if (line_prefix == "v")
		{
			std::vector<float> list = OBJ_append_content(line);
			glm::vec3 vertex = glm::vec3(list[0], list[1], list[2]);
			vertex_positions.push_back(vertex);

			// Passing in vertices RAW
			for (int i = 0; i < 3; i++)
			{
				//model_content.vertices.push_back(list[i]);
			}
		}

		else if (line_prefix == "vt")
		{
			std::vector<float> list = OBJ_append_content(line);
			// Have to do 1 - uv.y as opengl uv.y position is flipped
			glm::vec2 uvs = glm::vec2(list[0], list[1]);
			vertex_tex_coords.push_back(uvs);

			//std::cout << "Lines UV: " << list[0] << ", " << list[1] << "\n";
			//std::cout << "fixed Y pos: " << 1.f - (float)list[1] << "\n";

			//model_content.texture_Coordinates.push_back(list[0]);
			//model_content.texture_Coordinates.push_back(list[1]);
		}

		else if (line_prefix == "vn")
		{
			std::vector<float> list = OBJ_append_content(line);
			glm::vec3 normals = glm::vec3(list[0], list[1], list[2]);
			vertex_normals.push_back(normals);
		}

		else if (line_prefix == "f")
		{
			/*  --=-= FACE EXAMPLE =-=--
				f 2/1/1 3/2/1 1/3/1     # One Face - (3 Vertices)
				2 / 1 / 1  ==  VertexPosition(v) / TextureCoordinate(vt) / VertexNormal(vn)
				Note: first element is indices to VertexPositions. These are already in order
				although, it is necessary to re-organise the uv's & normals
			*/

			// Currently NOT organising UV's to index order

			OBJ_Face_3D face = OBJ_append_indices(line);


			/*
				ISSUE RESOLVED - (KINDA) :
				Using indices was creating an offset for texture coordinates to some degree (messing up the image)
				Currently not using indices.
				For optimisation learn using indices for texture coordinates
			*/


			// Organising data with face
			for (int i = 0; i < 3; i++)
			{
				// Vertex Position - (Index)
				model_content.indices.push_back(face.vertex_pos[i]);

				glm::vec3 face_vertice_pos = vertex_positions[face.vertex_pos[i]];
				model_content.vertices.push_back(face_vertice_pos.x);
				model_content.vertices.push_back(face_vertice_pos.y);
				model_content.vertices.push_back(face_vertice_pos.z);



				glm::vec2 face_texCoord = vertex_tex_coords[face.vertex_uv[i]];
				model_content.texture_Coordinates.push_back(face_texCoord.x);
				model_content.texture_Coordinates.push_back(1 - face_texCoord.y);


				glm::vec3 face_normal = vertex_normals[face.vertex_norm[i]];
				model_content.normals.push_back(face_normal.x);
				model_content.normals.push_back(face_normal.y);
				model_content.normals.push_back(face_normal.z);

			}

		}

	};


	return model_content;
}


void gen_texture(unsigned int& location, std::string image_file_name, unsigned int texture_location = 0)
{
	glActiveTexture(texture_location);
	glGenTextures(1, &location);
	glBindTexture(GL_TEXTURE_2D, location);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, bit_per_pixel;
	unsigned char* data = stbi_load(image_file_name.c_str(), &width, &height, &bit_per_pixel, 0);


	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, bit_per_pixel, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -1.5f);

		stbi_image_free(data);
		std::cout << "\n[Texture] Successfully loaded: '" << image_file_name << "'";
		TotalLoadedTextures++;
	}
	else
	{
		std::cout << "\n[Texture] Failed to load: '" << image_file_name << "'";
	}


}

void gen_texture_from_embedded(unsigned int& location, const aiTexture* texture, unsigned int texture_location = 0)
{
	glActiveTexture(texture_location);
	glGenTextures(1, &location);
	glBindTexture(GL_TEXTURE_2D, location);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	unsigned int bufferSize = texture->mWidth;

	int width, height, bit_per_pixel;
	void* data = stbi_load_from_memory((const stbi_uc*)texture->pcData, bufferSize, &width, &height, &bit_per_pixel, 0);


	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, bit_per_pixel, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -1.5f);

		stbi_image_free(data);
		std::cout << "\n[Texture] Successfully loaded: '" << texture->mFilename.data << "'";
		TotalLoadedTextures++;
	}
	else
	{
		std::cout << "\n[Texture] Failed to load: '" << texture->mFilename.data << "'";
	}
}




RawTexture read_MTL(std::string file_name)
{

	std::ifstream file(file_name);
	std::string current_line;

	RawTexture texture_content;



	std::string line;
	while (std::getline(file, current_line))
	{
		line = current_line;

		if (current_line.length() != 0)
		{
			unsigned int first_space = current_line.find(" ");
			auto line_prefix = current_line.erase(first_space);
			line.erase(0, first_space + 1);


			// Searching for ...:



			// File Name
			if (line_prefix == "map_Kd")
			{
				texture_content.file_names.push_back(TEXTURE_FOLDER + line);
			}

		}
	}


	return texture_content;
}




// Init Func - (Collects contents from specified file)
RawModelContent load_OBJ(std::string file, int file_type)
{

	std::string file_name = TEXTURE_FOLDER + file;
	RawModelContent model_content;

	if (file_type == (int)FILE_TYPES::OBJ)
	{
		// Reads & returns content from obj file
		model_content = read_OBJ(file_name + ".obj");

	}

	return model_content;
}


RawTexture load_MLT(std::string file, int file_type)
{
	std::string file_name = TEXTURE_FOLDER + file;
	RawTexture texture_content = { 0 };
	if (file_type == (int)FILE_TYPES::MTL)
	{
		texture_content = read_MTL(file_name + ".mtl");
	}

	// Amount of textures determined by checking how many images files are found
	texture_content.textures_count = texture_content.file_names.size();
	return texture_content;
}




// DONE ONCE PER MODEL
// PREDOMINANT VAO MUST BE INITIALLY ACTIVATED BEFORE FUNCTION IS CALLED
/* Will load model data into buffers */
void setup_buffers(Buffers& buffers, RawModelContent& model_content, RawTexture& texture_content)
{
	// References to make code more clear
	// ISSUE NOTE: buffers.<buffers> are not being set up. 
	unsigned int& vbo = buffers.vbo;
	unsigned int& ibo = buffers.ibo;
	unsigned int& tbo = buffers.tbo;

	std::vector<float>& vertices = model_content.vertices;
	std::vector<unsigned int>& indices = model_content.indices;
	std::vector<float>& uvs = model_content.texture_Coordinates;



	// Creating Vertex buffer
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);



	// Creating Index Buffer
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_DYNAMIC_DRAW);



	// Creating UVs buffer
	glGenBuffers(1, &tbo);
	glBindBuffer(GL_ARRAY_BUFFER, tbo);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(float), uvs.data(), GL_DYNAMIC_DRAW);

	// TEXTURE IS LOADED DURING FILE EXTRACTION ATM
	texture_content.model_textures = (unsigned int*)malloc(sizeof(unsigned int) * texture_content.file_names.size());
	texture_content.textures_count = texture_content.file_names.size();
	for (unsigned int i = 0; i < texture_content.textures_count; i++)
		gen_texture(texture_content.model_textures[i], texture_content.file_names[i], i);
}

// NOTES: Still trying to convert over to using program class. Models need to be set up 
void Model_Own::SetUp(std::string FileName)
{
	RawModelContent model_content;

	model_content = load_OBJ(FileName, (int)FILE_TYPES::OBJ);
	this->texture_content = load_MLT(FileName, (int)FILE_TYPES::MTL);

	this->model_content = model_content;
	// Fill buffers with content of a model 
	setup_buffers(this->buffers, model_content, this->texture_content);
	this->Set_Counts(model_content);
}

// MUST BE LOADED AT START
Model_Global::Model_Global()
{

}


// ALL ABOVE IS 1st PARTY WRITTEN
/* ASSIMP MODEL LOADING CODE */
const unsigned int DEFAULT_MODEL_SCALE = 1;

// Might be wise to extend "glm::mat4" to add these methods.
const glm::mat4 aiMat4x4_To_GlmMat4(aiMatrix4x4 aiMat) {
	return glm::mat4(\
		glm::vec4(aiMat.a1, aiMat.a2, aiMat.a3, aiMat.a4), \
		glm::vec4(aiMat.b1, aiMat.b2, aiMat.b3, aiMat.b4), \
		glm::vec4(aiMat.c1, aiMat.c2, aiMat.c3, aiMat.c4), \
		glm::vec4(aiMat.d1, aiMat.d2, aiMat.d3, aiMat.d4));
};
const aiMatrix4x4 GetScaleMatrix(float x, float y, float z)
{
	return aiMatrix4x4(\
		x, .0f, .0f, .0f, \
		.0f, y, .0f, .0f, \
		.0f, .0f, z, .0f, \
		.0f, .0f, .0f, 1.0f);
}
const aiMatrix4x4 GetTranslationMatrix(float x, float y, float z) {
	return aiMatrix4x4(\
		1.0f, .0f, .0f, x, \
		.0f, 1.0f, .0f, y, \
		.0f, .0f, 1.0f, z, \
		.0f, .0f, .0f, 1.0f);
}

// Main method for loading model data
void Model::LoadModel(const std::string& fileName, float scale = DEFAULT_MODEL_SCALE)
{
	// Generate models VAO
	glGenVertexArrays(1, &this->m_VAO);
	glBindVertexArray(this->m_VAO);


	// Maybe do for each meshes buffers
	// Generates buffers used for model
	glGenBuffers(ARRAY_COUNT(this->m_Buffers, GLuint), this->m_Buffers);

	// Gather Assimp-Model Scene

	this->pScene = this->Importer.ReadFile(TEXTURE_FOLDER + fileName, ASSIMP_LOAD_FLAGS);

	if (!pScene)
		printf("\n[ASSIMP]: Err parsing '%s' - '%s'", fileName.c_str(), this->Importer.GetErrorString());
	else
	{
		this->m_ObjFileName = fileName;
		
		if ((this->m_HasAnimations = this->pScene->HasAnimations()))
		{
			this->m_GlobalInverseTransform = this->pScene->mRootNode->mTransformation;
			this->m_GlobalInverseTransform.Inverse();
		}


		this->InitialiseMeshesFromScene(this->pScene);
		this->SetupBuffers();
		this->m_ModelScale = scale;
	}

	// Unbinding VAO
	glBindVertexArray(0);
}

bool Model::InitialiseMeshesFromScene(const aiScene* pScene)
{
	this->m_Meshes.resize(pScene->mNumMeshes);
	this->m_MeshesCount = pScene->mNumMeshes;
	
	
	this->SetCounts(pScene);
	this->ReserveArrays();
	this->ExtractMeshesData(pScene);
	this->ExtractMaterialData(pScene);


	return true;
}

void Model::SetCounts(const aiScene* pScene)
{
	// Iterate through each MESH in model. Add meshes counts to m_meshes array & increment total counts
	for (unsigned int i = 0; i < this->m_Meshes.size(); i++)
	{
		m_Meshes[i].NumIndices = pScene->mMeshes[i]->mNumFaces * 3;    // Triangle has 3 Indices/Points
		m_Meshes[i].MaterialIndex = pScene->mMeshes[i]->mMaterialIndex;

		// MESHES BASE index for Indices & Vertices - (EXAMPLE: 3rd meshes offset/base vertex = 1st + 2nd meshes vertice count and so on for n'th mesh...)
		m_Meshes[i].BaseVertex = this->m_VerticesCount;
		m_Meshes[i].BaseIndex = this->m_IndicesCount;

		// Adds meshes vertices count onto total verticesCount each iteration
		this->m_VerticesCount += pScene->mMeshes[i]->mNumVertices;
		this->m_IndicesCount += m_Meshes[i].NumIndices;
	}
}

// Used for keep track of current active mesh during Initalising phases 
static unsigned int Global_Current_Mesh_ID = 0;

void Model::ExtractMeshesData(const aiScene* pScene)
{
	for (unsigned int i = 0; i < this->m_Meshes.size(); i++)
	{
		Global_Current_Mesh_ID = i;
		const aiMesh* paiMesh = pScene->mMeshes[i];
		this->InitialiseSingleMesh(paiMesh);
	}
}

void Model::InitialiseSingleMesh(const aiMesh* paiMesh)
{

	const aiVector3D EmptyUV(0, 0, 0);
		

	for (unsigned int i = 0; i < paiMesh->mNumVertices; i++)
	{
		const aiVector3D& pPos = paiMesh->mVertices[i];
		const aiVector3D& pNormal = paiMesh->mNormals[i];
		const aiVector3D& pUV = paiMesh->HasTextureCoords(0) ? paiMesh->mTextureCoords[0][i] : EmptyUV;
	

		this->m_Vertices.push_back(Vector3(pPos.x, pPos.y, pPos.z));
		this->m_Normals.push_back(Vector3(pNormal.x, pNormal.y, pNormal.z));
		this->m_UVs.push_back(Vector2(pUV.x, pUV.y));
	}
	for (unsigned int i = 0; i < paiMesh->mNumFaces; i++)
	{
		const aiFace& pFace = paiMesh->mFaces[i];
		for (unsigned int j = 0; j < 3; j++)
			m_Indices.push_back(pFace.mIndices[j]);
	}
	if (paiMesh->HasBones())
		this->ExtractMeshBones(paiMesh);
}

void Model::ExtractMeshBones(const aiMesh* paiMesh)
{
	for (unsigned int i = 0; i < paiMesh->mNumBones; i++)
	{
		aiBone* paiBone = paiMesh->mBones[i];
		this->InitialiseSingleMeshBone(paiBone);
	}
}



void Model::InitialiseSingleMeshBone(const aiBone* pBone)
{
	unsigned int meshBaseVertex = this->m_Meshes[Global_Current_Mesh_ID].BaseVertex;
	
	
	if (DEBUG)
		printf("\nBone Name: %s - Weights Count (%u) - Bone Weights: ", pBone->mName.C_Str(), pBone->mNumWeights);
	

	for (unsigned int i = 0; i < pBone->mNumWeights; i++)
	{
		aiVertexWeight& weight = pBone->mWeights[i];
		
		// OFFSET FOR VERTICES WITHIN THIS CURRENT MESH
		unsigned int vertexIDInVAO = weight.mVertexId + meshBaseVertex;

		//if (DEBUG)
		//	printf("\n[Vertex ID %u]: Influence (%f)", vertexIDInVAO, weight.mWeight);
		
	
		int boneID = this->GetBoneID(pBone);
		this->m_Bones[vertexIDInVAO].AddData(boneID, weight.mWeight);
	}
	
}


/*This is called at runtime.
  will return array of matrices "Transformations" to be sent over to shader as a uniform*/
std::vector<Matrix4f> Model::GetCurrentBoneTransforms(float CurrentTime)
{
	std::vector<Matrix4f> transformations;
	transformations.resize(this->m_BonesInfo.size());

	// Change when using multiple animations
	const unsigned int animationsIndex = 0;
	//auto test = this->pScene->mNumAnimations;
	float animationsTicksPerSecond = (float)(this->pScene->mAnimations[animationsIndex]->mTicksPerSecond != 0 ? this->pScene->mAnimations[animationsIndex]->mTicksPerSecond : this->DEFAULT_ANIMATION_TICKS_PER_SECOND);
	float timeInTicks = CurrentTime * animationsTicksPerSecond;
	float animationTimeInTicks = fmod(timeInTicks, (float)this->pScene->mAnimations[animationsIndex]->mDuration);

	Matrix4f identity;
	identity.InitIdentity();
	
	this->ReadNodeHeirarchy(this->pScene->mRootNode, identity, animationTimeInTicks);
	
	for (unsigned int i = 0; i < this->m_BonesInfo.size(); i++)
	{
		transformations[i] = this->m_BonesInfo[i].FinalTransformation;
	}

	return transformations;
}


void Model::ReadNodeHeirarchy(const aiNode* pNode, const Matrix4f ParentMat, float AnimationTime)
{
	std::string NodeName(pNode->mName.data);

	const aiAnimation* pAnimation = pScene->mAnimations[0];

	Matrix4f NodeTransformation(pNode->mTransformation);
	

	const aiNodeAnim* pNodeAnim;
	pNodeAnim = GetCurrentNodeAnimation(pAnimation, NodeName);
	

	if (pNodeAnim) {
		
		aiVector3D Scaling;
		CalculateInterpolatedScaling(Scaling, pNodeAnim, AnimationTime);
		Matrix4f ScalingM;
		ScalingM.InitScaleTransform(Scaling.x, Scaling.y, Scaling.z);

		
		aiQuaternion RotationQ;
		CalculateInterpolatedRotation(RotationQ, pNodeAnim, AnimationTime);
		Matrix4f RotationM = Matrix4f(RotationQ.GetMatrix());

		
		aiVector3D Translation;
		CalculateInterpolatedPositioning(Translation, pNodeAnim, AnimationTime);
		Matrix4f TranslationM;
		TranslationM.InitTranslationTransform(Translation.x, Translation.y, Translation.z);

	
		NodeTransformation = TranslationM * RotationM * ScalingM;
	}
	

	Matrix4f GlobalTransformation = ParentMat * NodeTransformation;

	if (m_BoneName_To_BoneID.find(NodeName) != m_BoneName_To_BoneID.end()) {
		unsigned int BoneIndex = m_BoneName_To_BoneID[NodeName];
		m_BonesInfo[BoneIndex].FinalTransformation = m_GlobalInverseTransform * GlobalTransformation *
			m_BonesInfo[BoneIndex].OffsetMatrix;
	}

	for (unsigned int i = 0; i < pNode->mNumChildren; i++) {
		ReadNodeHeirarchy(pNode->mChildren[i], GlobalTransformation, AnimationTime);
	}

};
// Will return NULL if not aiNodeAnim not found
const aiNodeAnim* Model::GetCurrentNodeAnimation(const aiAnimation* animation, std::string nodeName)
{
	for (unsigned int  i = 0; i < animation->mNumChannels; i++) {
		const aiNodeAnim* pNodeAnim = animation->mChannels[i];

		if (std::string(pNodeAnim->mNodeName.data) == nodeName) {
			return pNodeAnim;
		}
	}

	return NULL;
}
void Model::CalculateInterpolatedScaling(aiVector3D& r_Vec, const aiNodeAnim* pNodeAnimation, float AnimationTime)
{
	/* SCALING */
	// Should be atleast 2 values for interpolation
	if (pNodeAnimation->mNumScalingKeys == 1) {
		r_Vec = pNodeAnimation->mScalingKeys[0].mValue;
		return;
	}

	unsigned int keyTypeIndex = this->GetCurrentScalingKeyIndex(pNodeAnimation, AnimationTime);
	unsigned int neighbourKeyTypeIndex = keyTypeIndex + 1;

	aiVectorKey& key1 = pNodeAnimation->mScalingKeys[keyTypeIndex];
	aiVectorKey& key2 = pNodeAnimation->mScalingKeys[neighbourKeyTypeIndex];

	float time1 = (float)key1.mTime;
	float time2 = (float)key2.mTime;
	float deltaTime = time2 - time1;
	float factor = (AnimationTime - time1 / deltaTime);

	const aiVector3D& start = key1.mValue;
	const aiVector3D& end = key2.mValue;
	aiVector3D delta = end - start;
	r_Vec = start + factor * delta;
}
void Model::CalculateInterpolatedRotation(aiQuaternion& r_Quat, const aiNodeAnim* pNodeAnimation, float AnimationTime)
{
	// we need at least two values to interpolate...
	if (pNodeAnimation->mNumRotationKeys == 1) {
		r_Quat = pNodeAnimation->mRotationKeys[0].mValue;
		return;
	}

	unsigned int keyTypeIndex = GetCurrentRotationKeyIndex(pNodeAnimation, AnimationTime);
	unsigned int neighbourKeyTypeIndex = keyTypeIndex + 1;

	const aiQuatKey& key1 = pNodeAnimation->mRotationKeys[keyTypeIndex];
	const aiQuatKey& key2 = pNodeAnimation->mRotationKeys[neighbourKeyTypeIndex];

	float DeltaTime = (float)(key2.mTime - key1.mTime);
	float Factor = (AnimationTime - (float)key1.mTime) / DeltaTime;
	const aiQuaternion& StartRotationQ = key1.mValue;
	const aiQuaternion& EndRotationQ =   key2.mValue;
	aiQuaternion::Interpolate(r_Quat, StartRotationQ, EndRotationQ, Factor);
	r_Quat = r_Quat.Normalize();
}


void Model::CalculateInterpolatedPositioning(aiVector3D& r_Vec, const aiNodeAnim* pNodeAnimation, float AnimationTime)
{
	/* TRANSFORMING/POSITIONING */
	// Should be atleast 2 values for interpolation
	if (pNodeAnimation->mNumPositionKeys == 1)
	{
		r_Vec = pNodeAnimation->mPositionKeys[0].mValue;
		return;
	}

	unsigned int keyTypeIndex = this->GetCurrentPositionKeyIndex(pNodeAnimation, AnimationTime);
	unsigned int neighbourKeyTypeIndex = keyTypeIndex + 1;

	aiVectorKey& key1 = pNodeAnimation->mPositionKeys[keyTypeIndex];
	aiVectorKey& key2 = pNodeAnimation->mPositionKeys[neighbourKeyTypeIndex];

	float time1 = (float)key1.mTime;
	float time2 = (float)key2.mTime;
	float deltaTime = time2 - time1;
	float factor = (AnimationTime - time1 / deltaTime);

	const aiVector3D& start = key1.mValue;
	const aiVector3D& end = key2.mValue;
	aiVector3D delta = end - start;
	r_Vec = start + factor * delta;
}


unsigned int Model::GetCurrentScalingKeyIndex(const aiNodeAnim* pNodeAnimation, float AnimationTime)
{
	assert(pNodeAnimation->mNumScalingKeys > 0);

	for (unsigned int i = 0; i < pNodeAnimation->mNumScalingKeys - 1; i++) {
		if (AnimationTime < (float)pNodeAnimation->mScalingKeys[i + 1].mTime) {
			return i;
		}
	}
	assert(0);
	return 0;
}

unsigned int Model::GetCurrentRotationKeyIndex(const aiNodeAnim* pNodeAnimation, float AnimationTime)
{
	assert(pNodeAnimation->mNumRotationKeys > 0);

	for (unsigned int i = 0; i < pNodeAnimation->mNumRotationKeys - 1; i++) {
		if (AnimationTime < (float)pNodeAnimation->mRotationKeys[i + 1].mTime) {
			return i;
		}
	}

	assert(0);
	return 0;
}
unsigned int Model::GetCurrentPositionKeyIndex(const aiNodeAnim* pNodeAnimation, float AnimationTime)
{
	for (unsigned int i = 0; i < pNodeAnimation->mNumPositionKeys - 1; i++) {
		if (AnimationTime < (float)pNodeAnimation->mPositionKeys[i + 1].mTime) {
			return i;
		}
	}
	assert(0);
	return 0;
}

 
int Model::GetBoneID(const aiBone* pBone)
{
	int boneID = -1;
	aiString BoneName = pBone->mName;

	if (this->m_BoneName_To_BoneID.find(BoneName.C_Str()) == m_BoneName_To_BoneID.end())
	{  
		// Than this bone has JUST been created
		boneID = this->m_BoneName_To_BoneID.size();
		// Add new item - Key=BoneName,  Val=(New ID - SAME AS: Quantity of items + 1)
		this->m_BoneName_To_BoneID[BoneName.C_Str()] = boneID;
		BoneInfo boneInfo = BoneInfo(pBone->mOffsetMatrix);
		this->m_BonesInfo.push_back(boneInfo);
	}
	else
		// Else BoneName IS found in map
		boneID = this->m_BoneName_To_BoneID[BoneName.C_Str()];

	return boneID;
};

void Model::ExtractMaterialData(const aiScene* pScene)
{
	// Set an m_Materials maybe 
	// Currently, a texture-slot is reserved for each MATERIAL - (If a material has no texture, a slot will be resevered anyway, at the momment)
	this->m_Textures = (unsigned int*)malloc(sizeof(unsigned int) * pScene->mNumTextures);
	this->m_MaterialsCount = pScene->mNumMaterials;
	


	// Iterate through all identified materials
	for (unsigned int i = 0; i < pScene->mNumMaterials; i++)
	{
		const aiMaterial* pMaterial = pScene->mMaterials[i];

		// If textures found
		if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			aiString path;
			if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
			{			

				const aiTexture* paiTexture = this->pScene->GetEmbeddedTexture(path.data);
				if (paiTexture)
				{
					gen_texture_from_embedded(this->m_Textures[this->m_TextureCount], paiTexture, i);
				}
				else 
				{
					std::string imgFileName(path.data);
					std::string fullPath(TEXTURE_FOLDER + imgFileName);
					gen_texture(this->m_Textures[i], fullPath, i);
				}


				this->m_TextureCount++;
			}
		}

	}
}

void Model::SetupBuffers()
{
	// VAO data
	unsigned int& vbo = this->m_Buffers[ENUM_UINT(BUFFER_TYPE::VERTEX)];
	unsigned int& tbo = this->m_Buffers[ENUM_UINT(BUFFER_TYPE::UV)];
	unsigned int& nbo = this->m_Buffers[ENUM_UINT(BUFFER_TYPE::NORMAL)];
	
	/*Bones Buffer Object contains array of VertexBoneData for each vertex 
		VertexBonesData contains
		- BoneId's vertex is influenced by
		- Weights for each boneId
	*/
	unsigned int& bbo = this->m_Buffers[ENUM_UINT(BUFFER_TYPE::BONES)];
	
	// Index data
	unsigned int& ibo = this->m_Buffers[ENUM_UINT(BUFFER_TYPE::INDEX)];


	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, this->m_Vertices.size() * sizeof(this->m_Vertices[0]), &this->m_Vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(ENUM_UINT(VAO_BUFFER_LOCATIONS::VERTEX));
	glVertexAttribPointer(ENUM_UINT(VAO_BUFFER_LOCATIONS::VERTEX), 3, GL_FLOAT, GL_FALSE, 0, 0);


	glBindBuffer(GL_ARRAY_BUFFER, tbo);
	glBufferData(GL_ARRAY_BUFFER, this->m_Vertices.size() * sizeof(this->m_UVs[0]), &this->m_UVs[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(ENUM_UINT(VAO_BUFFER_LOCATIONS::UV));
	glVertexAttribPointer(ENUM_UINT(VAO_BUFFER_LOCATIONS::UV), 2, GL_FLOAT, GL_FALSE, 0, 0);


	glBindBuffer(GL_ARRAY_BUFFER, nbo);
	glBufferData(GL_ARRAY_BUFFER, this->m_Vertices.size() * sizeof(this->m_Normals[0]), &this->m_Normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(ENUM_UINT(VAO_BUFFER_LOCATIONS::NORMAL));
	glVertexAttribPointer(ENUM_UINT(VAO_BUFFER_LOCATIONS::NORMAL), 3, GL_FLOAT, GL_FALSE, 0, 0);

	if (this->m_HasAnimations)
	{
		glBindBuffer(GL_ARRAY_BUFFER, bbo);
		glBufferData(GL_ARRAY_BUFFER, this->m_Bones.size() * sizeof(this->m_Bones[0]), &this->m_Bones[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(ENUM_UINT(VAO_BUFFER_LOCATIONS::BONES));
		glVertexAttribIPointer(ENUM_UINT(VAO_BUFFER_LOCATIONS::BONES), MAX_BONES_PER_VERTEX, GL_INT, sizeof(VertexBoneData), 0);
		glEnableVertexAttribArray(ENUM_UINT(VAO_BUFFER_LOCATIONS::WEIGHTS));
		// Specify OFFSET - Location::BONES = BoneID's array THAN Location::WEIGHTS = Weights (which is located at offset of +BoneID's array 
		glVertexAttribPointer(ENUM_UINT(VAO_BUFFER_LOCATIONS::WEIGHTS), MAX_BONES_PER_VERTEX, GL_FLOAT, GL_FALSE, 
			sizeof(VertexBoneData), (const GLvoid*)(MAX_BONES_PER_VERTEX * sizeof(int32_t)));
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->m_Indices.size() * sizeof(unsigned int), this->m_Indices.data(), GL_DYNAMIC_DRAW);
}


void Model::ReserveArrays()
{
	this->m_Vertices.reserve(this->m_VerticesCount);
	this->m_Normals.reserve(this->m_VerticesCount);
	this->m_UVs.reserve(this->m_VerticesCount);
	this->m_Indices.reserve(this->m_IndicesCount);

	if (this->m_HasAnimations)
		this->m_Bones.resize(this->m_VerticesCount);
}



void Model::AttachModelsVAO()
{
	glBindVertexArray(this->m_VAO);
}
void Model::AttachMaterialsTextures(unsigned int location = 0)
{
	// With mutlitple textures: For loop for each texture below?
	// Activate Texture Slot
	// For now, only binding one texture per model

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->m_Textures[location]);
}



Model::Model()
	:
	m_VAO(NULL), m_Buffers(),
	m_Meshes(std::vector<MeshEntry>()),
	m_VerticesCount(0), m_IndicesCount(0), m_MaterialsCount(0), m_MeshesCount(0), m_TextureCount(0), 
	m_Textures(NULL), m_ModelScale(NULL)
	
{}