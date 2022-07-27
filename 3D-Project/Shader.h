#pragma once
#include "GlobalItems.h"

class Shader
{
public:
	unsigned int shader;

	/* Create instance for a shader */
	Shader();
	
	/* Attach uniforms to shader */
	void AttachUniforms();
	void UpdateUniforms();
	void Bind();
	
	// Uniforms
public:
	int u_modelMatrix;

	int u_view;
	int u_projection;
	int u_cam_pos;
};
