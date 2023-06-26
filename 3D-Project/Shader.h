#pragma once
#include "GlobalItems.h"
#include "Camera.h"

const std::string SHADER_FOLDER_DIR = "3D-Project\\Shaders\\";

class Shader
{
public:
	unsigned int shader;

	/* Create instance for a shader */
	Shader();
	void SetUp(std::string file_name);

	/* Attach uniforms to shader */
	void AttachUniforms();
	void UpdateUniforms();
	void Bind();
	


	// Uniforms
public:	
	int u_modelMatrix;

	// Array of Textures THEN Maps
	int* u_samplers;
	unsigned int textures_count;
	unsigned int maps_count;

	int u_view;
	int u_projection;
	int u_cam_pos;
	void UpdateCameraUniforms(Camera* camera);

	// Uniforms used Animated Models
public:
	// Max bones is current 100
	int m_u_Bones[100];
};



// Shader class used for models - (Both animated & non-animated models extend from this class)
class ObjectShader : public Shader
{
	// Extended Properties
private:
	int u_isTextured;

	 
public:
	void ExtendedSetUp(std::string file_name);
	void UpdateIsTexturedUniform(bool);


	// Constructors
public:
	ObjectShader();
};


/* #-#-#- Shader Specific Classes -#-#-# */

// Animated Models Shader
// This shader consists of usage of - (not limited too these):
// - Bones Matrices Array of model 
// - BoneID's in VAO which influence a Vertex
// - Weight's in VAO which declare the intensity of which a bone influences the vertex
class AnimatedModelShader : public ObjectShader 
{
public:
	void LoadShader(std::string file_name);

public:
	// only used for main.glsl
	void SetBonesUniform();
	

	// Constructors
public:
	AnimatedModelShader();
};


// Non-animated Models Shader
class PlainModelShader : public ObjectShader
{
public:
	void LoadShader(std::string file_name);
public:
	PlainModelShader();
};


class TerrainShader : public Shader
{
public:
	TerrainShader();
};


class CubeMapShader : public Shader
{
public:
	CubeMapShader();
};