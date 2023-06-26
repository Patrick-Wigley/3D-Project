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
	// only used for main.glsl
	void SetBonesUniform();
	// Max bones is current 100
	int m_u_Bones[100];
	
	int u_modelMatrix;

	// Array of Textures THEN Maps
	int* u_samplers;
	unsigned int textures_count;
	unsigned int maps_count;

	int u_view;
	int u_projection;
	int u_cam_pos;
	void UpdateCameraUniforms(Camera* camera);
};



// NOTE: If wish to extend properties such as uniforms of other shaders, create sub class as shown below:
// This is the "MainShader". This has extended properties
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