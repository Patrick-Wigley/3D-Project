#include "globalitems.h"



// Main game Camera
class Camera
{
private:
	float yaw, pitch;
	float pos[3];
	float previous_x;
	float previous_y;

	glm::vec3 direction;

	const float sensitivity = .1f;

	// Matrices
private:
	glm::mat4 view;
	glm::mat4 rotation;
	// NOTE: projection Matrix is setup once
	glm::mat4 projection;

	// Shaders Uniforms
private:
	int u_view;
	int u_pos;
	int u_projection;

	// Update Members
private:
	void DirectionUpdate();
	void KeyUpdate();

	// Ptr to window instance
private:
	Window* p_window;


public:
	const void Update();
	Camera(Window* win, unsigned int& shader, int win_size[]);

};