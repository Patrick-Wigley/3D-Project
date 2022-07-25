#include "Camera.h"
#include "glm/gtx/string_cast.hpp"

const void Camera::Update()
{
	this->DirectionUpdate();
	this->KeyUpdate();

	// Update Matrices
	//glm::vec3 position_vec = glm::vec3(pos[0], pos[1], pos[2]);
	//this->view = glm::lookAt(position_vec, position_vec + direction, glm::vec3(0, 1, 0));
	//this->rotation = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0) + direction, glm::vec3(0, 1, 0));


	// Update Uniforms
	glUniformMatrix4fv(u_projection, 1, false, glm::value_ptr(projection));
	glUniformMatrix4fv(u_view, 1, false, glm::value_ptr(view));
	glUniform3f(u_pos, pos[0], pos[1], pos[2]);

};



void Camera::DirectionUpdate()
{
	double mouse_x, mouse_y;
	glfwGetCursorPos(this->p_window, &mouse_x, &mouse_y);

	this->yaw += (mouse_x - this->previous_x) * this-> sensitivity;
	this->pitch += (this->previous_y - mouse_y) * this->sensitivity;

	this->previous_x = mouse_x;
	this->previous_y = mouse_y;

	glm::vec3 camDir;
	camDir.x = cos(yaw) * cos(pitch);
	camDir.y = sin(pitch);
	camDir.z = sin(yaw) * cos(pitch);

	direction = camDir;
};

void Camera::KeyUpdate()
{

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