#include "globalitems.h"
#include "Shader.h"


// Main game Camera
class Camera
{
private:
	float yaw, pitch;
	float pos[3];
	float previous_x;
	float previous_y;

	glm::vec3 direction;

	const float sensitivity = .01f;

	// Adjustable factors
private:
	float movement_speed = 1;


	// Matrices
private:
	glm::mat4 view;
	glm::mat4 rotation;
	// NOTE: projection Matrix is setup once
	glm::mat4 projection;


	// Shaders Uniforms References
private:
	int& r_u_view;
	int& r_u_pos;
	int& r_u_projection;

	// Update Members
private:
	void DirectionUpdate();
	void KeyUpdate();

	// Ptr to window instance
private:
	Window* p_window;


public:
	const void Update();
	Camera(Window* win, Shader& shader, int win_size[]);

};