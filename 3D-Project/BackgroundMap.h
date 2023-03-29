#include "GlobalItems.h"
#include "Shader.h"

/* SkyMap has its own VAO*/
class SkyMap
{
public:
	SkyMap();
	
	void SetUp();
	void Draw(const glm::mat4* rotation_matrix, const glm::mat4* projection_matrix);

private:
	unsigned int vao;
	unsigned int tbo;
	unsigned int vbo;
	void Update();


	Shader shader;
	unsigned int& r_shader;

	// Uniforms
	int u_view;
	int u_projection;
	int u_sampler_skybox;
};

