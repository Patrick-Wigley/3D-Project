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

	void Bind();
	
};
