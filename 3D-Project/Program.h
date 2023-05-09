
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
	Camera m_Camera;
	SkyMap m_CubeMap;


	/* Vao's & Shaders */
private:
	// Predominant VAO
	unsigned int m_PredominantVAO;
	
	AnimatedModelShader m_Shader_AnimModels;
	PlainModelShader m_Shader_PlainModels;
	TerrainShader m_Shader_Terrain;



	/* Entity Arrays */
private:
	std::vector<DynamicObj> bullets{};


	/* CONST Models & Terrains */
private:
	Model_Global model_nappy_guy;
	Model_Global model_energy_ball;


	/* MODELS */
	// Animated
	Model m_Model_Entity;
	Model m_Model_Character;
	Model m_Model_DancingGuy;
	// Non Animated
	Model m_Model_NonAnimatedModel;
	Model m_Model_EnergyBall;

	/* TERRAINS */
	// Terrains differe from models as they have seperate methods such as collision detection
	Terrain m_Model_Terrain;



private:
	long long m_ProgramStartTimeInMill;
	float GetCurrentTimeInTicks();	


	// Objs
private:
	std::vector<Light> m_Light_LightSources;



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