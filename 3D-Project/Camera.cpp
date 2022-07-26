#include "Camera.h"
#include "glm/gtx/string_cast.hpp"


constexpr static int X = 0;
constexpr static int Y = 1;
constexpr static int Z = 2;



const void Camera::Update()
{
	this->DirectionUpdate();
	this->KeyUpdate();

	// Update Matrices
	glm::vec3 position_vec = glm::vec3(pos[X], pos[Y], pos[Z] - 5);
	this->view = glm::lookAt(position_vec, position_vec + direction, glm::vec3(0, 1, 0));
	//this->rotation = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0) + direction, glm::vec3(0, 1, 0));


	// Update Uniforms
	glUniformMatrix4fv(u_projection, 1, false, glm::value_ptr(projection));
	glUniformMatrix4fv(u_view, 1, false, glm::value_ptr(view));
	glUniform3f(u_pos, pos[X], pos[Y], pos[Z]);

};



void Camera::DirectionUpdate()
{
	double mouse_x, mouse_y;
	glfwGetCursorPos(this->p_window, &mouse_x, &mouse_y);

	this->yaw += (mouse_x - this->previous_x) * this-> sensitivity;
	this->pitch += (this->previous_y - mouse_y) * this->sensitivity;

	this->previous_x = mouse_x;
	this->previous_y = mouse_y;


	// Y Rotation Lock
	if (pitch > 1.5f)
		pitch = 1.5f;
	else if (pitch < -1.5f)
		pitch = -1.5f;

	glm::vec3 camDir;
	camDir.x = cos(yaw) * cos(pitch);
	camDir.y = sin(pitch);
	camDir.z = sin(yaw) * cos(pitch);

	direction = camDir;
};

void Camera::KeyUpdate()
{

	if (glfwGetKey(this->p_window, GLFW_KEY_W))
	{
		this->pos[X] += cos(yaw) / (15-this->movement_speed);
		this->pos[Z] += sin(yaw) / (15-this->movement_speed);
	}

	else if (glfwGetKey(this->p_window, GLFW_KEY_S))
	{
		this->pos[X] -= cos(yaw) / (15-this->movement_speed);
		this->pos[Z] -= sin(yaw) / (15-this->movement_speed);
	}

	if (glfwGetKey(this->p_window, GLFW_KEY_D))
	{
		this->pos[X] += cos(yaw + 1.6f) / 15;
		this->pos[Z] += sin(yaw + 1.6f) / 15;
	}

	else if (glfwGetKey(this->p_window, GLFW_KEY_A))
	{
		this->pos[X] -= cos(yaw + 1.6f) / 15;
		this->pos[Z] -= sin(yaw + 1.6f) / 15;
	}
	
};


Camera::Camera(Window* window, unsigned int& shader, int window_size[])
	: p_window(window),
	// Uniforms
	u_view(glGetUniformLocation(shader, "u_View")),
	u_projection(glGetUniformLocation(shader, "u_Projection")),
	u_pos(glGetUniformLocation(shader, "u_CameraPos"))
	// NOTE: projection Matrix is setup once

{
	projection = glm::perspective(glm::radians(60.0f), (float)(window_size[0] / window_size[1]), 0.1f, 100.0f);
	//std::cout << glm::to_string(projection) << std::endl;
	view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
	

}