#pragma once
#include "GlobalItems.h"

// For now this is just for an instance/object to use containing its own data regarding vertices etc

const static float VERTICES[]
{
	-0.5f,  0.5,  -0.5f,		// Top Left				0
	 0.5f,  0.5,  -0.5f,		// Top Right			1
	-0.5f, -0.5f, -0.5f,		// Bottom Left			2
	 0.5f, -0.5f, -0.5f,		// Bottom Right			3
};


// Indices
const unsigned int INDICES[]
{
    0,1,2,
	2,1,3
};



class Obj
{
private:
	unsigned int vao;
	unsigned int vertexBuffer;
	unsigned int ibo;
	
	
	// Potentially Protected fields if this becomes a super-class:
	void AttachBufferData();
	

public:
	Obj();
	
	void Bind();
};