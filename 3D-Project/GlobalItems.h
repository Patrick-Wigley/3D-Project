/* Includes  */
// Assimp Model Reader
#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>

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

#include <map>



typedef GLFWwindow Window;

typedef glm::vec3 Vector3;
typedef glm::vec2 Vector2;


/* Macros */ 
#define ARRAY_COUNT(arr, type) (sizeof(arr)/sizeof(type))

#define ENUM_UINT(e) (static_cast<unsigned int>(e))


// Assimp
#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices)





//struct GLWINDOW_PROPERTIES
//{
//	// May become dynamic if screen-size manipulation is implemented
//	int width = 1, height = 1;
//
//	int size = this->width * this->height;
//};
//
