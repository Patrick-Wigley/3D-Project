
#include "GlobalItems.h"
#include "Obj.h"

#include "Shader.h"
#include "BackgroundMap.h"

// Stores created meshes
#include "Meshes.h"




class Program
{
private:
	GLFWwindow* window;
	int* window_size;



	/* Singles */
private:
	Camera camera;
	SkyMap cubeMap;


	/* Static Vao's & Shaders */
private:
	// Predominant VAO
	unsigned int vao;
	// Predominant Shader
	Shader main_shader;
	Shader terrain_shader;


	/* Entity Arrays */
private:
	std::vector<BulletObj> bullets{};


	/* CONST Models & Terrains */
private:
	Model_Chest		model_chest;
	Model_BinGus	model_bin_gus;
	Model_NappyGuy	model_nappy_guy;

	Terrain model_terrain;


	/* Main Functions */
public:
	int SetUp();
	int ModelsSetUp();
	int EntitySetUp();
	void MainLoop();
	
	void QueryErrors();

	// NOT CURRENTLY USED 
	int WindowSetUp();

	/* Constructors */
public:
	Program(Window* window, int DEFAULT_WINDOW_SIZE[]);
	~Program();
};