
#include "GlobalItems.h"
#include "Obj.h"

#include "Shader.h"
#include "BackgroundMap.h"

// Stores created meshes
#include "Models.h"




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
	ObjectShader main_shader;
	TerrainShader terrain_shader;


	/* Entity Arrays */
private:
	std::vector<BulletObj> bullets{};


	/* CONST Models & Terrains */
private:
	Model_Global model_nappy_guy;
	Model_Global model_energy_ball;

	Terrain model_terrain;

	Model m_ModelEntity; // First test model
	Model m_ModelCharacter;

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