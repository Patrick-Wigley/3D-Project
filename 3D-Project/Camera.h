#include "globalitems.h"
//#include "Shader.h"

const float DEFAULT_JUMP_SPEED = 1.2f;

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
	float jump_speed = DEFAULT_JUMP_SPEED;


	// States
private:
	bool jumping = false;

	// Matrices
private:
	glm::mat4 view;
	glm::mat4 rotation;
	// NOTE: projection Matrix is setup once
	glm::mat4 projection;

	// Getters for matrices
public:
	const glm::mat4* GetRotationMatrix = &this->rotation;
	const glm::mat4* GetProjectionMatrix = &this->projection;
	const glm::mat4* GetView = &this->view;

	// Shaders Uniforms References
private:
	//int& r_u_view;
	//int& r_u_pos;
	//int& r_u_projection;

	// Update Members
private:
	void DirectionUpdate();
	void KeyUpdate();
	void JumpUpdate(float current_terrain_y);

	// Ptr to window instance
private:
	Window* p_window;


public:
	const void Update(float terrain_pos_height);
	Camera(Window* win, int win_size[]);


private:
	glm::vec3 camDir;
public:
	float* get_pos = this->pos;
	glm::vec3* Get_Rotation = &this->camDir;
};