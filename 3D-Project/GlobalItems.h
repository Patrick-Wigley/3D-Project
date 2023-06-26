/* Includes  */
// Assimp Model Reader
#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>
#include <Assimp/version.h>

// Graphics Libraries
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>


// Needed for file reading
#include <string>
#include <sstream>
#include <fstream>



// For debug outputs
#include <iostream>
#include <format>

// Data Structures
#include <map>

// SystemInfo - (Needs "Kernal.dll" & "Kernal.lib")
#include <Windows.h>

// Just testing linking this head file here. May need to fix includes by putting them in here.
#include "Lighting.h"


// TypeDefs
typedef GLFWwindow Window;

typedef glm::vec3 Vector3;
typedef glm::vec2 Vector2;
// #-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#


/* Macros */ 
#define ARRAY_COUNT(arr, type) (sizeof(arr)/sizeof(type))

#define ENUM_UINT(e) (static_cast<unsigned int>(e))

//static unsigned int Ticks = 0;
//#define TIMER(TicksLimit) ((Ticks >= TicksLimit))
//#define RESET_TIMER() (Ticks=0)


// Assimp
#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices | aiProcess_OptimizeGraph)
#define MAX_BONES 100
// Assimp Scaling keys - (Used when interpolating node matrices using aiVectorKey)
#define SCALE_KEY = 0
#define POSITION_KEY = 1
#define ROTATION_KEY = 2
// #-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#



//struct GLWINDOW_PROPERTIES
//{
//	// May become dynamic if screen-size manipulation is implemented
//	int width = 1, height = 1;
//
//	int size = this->width * this->height;
//};
//
