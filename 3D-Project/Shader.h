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
};

