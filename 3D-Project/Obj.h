#pragma once
#include "GlobalItems.h"

// For now this is just for an instance/object to use containing its own data regarding vertices etc

const static int VERTICE_SIZE = 3;
const static float VERTICES[]
{
								// FRONT
	-0.5f,  0.5,  -0.5f,		// Top Left				0
	 0.5f,  0.5,  -0.5f,		// Top Right			1
	-0.5f, -0.5f, -0.5f,		// Bottom Left			2
	 0.5f, -0.5f, -0.5f,		// Bottom Right			3
		
								// BACK
	-0.5f,  0.5,  -0.1f,		// Top Left				4
	 0.5f,  0.5,  -0.1f,		// Top Right			5
	-0.5f, -0.5f, -0.1f,		// Bottom Left			6
	 0.5f, -0.5f, -0.1f,		// Bottom Right			7
};


// Indices
const unsigned int INDICES[]
{
	// Front Face
    0,1,2,
	2,1,3,

	// Back Face
	4,5,6,
	6,5,7

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