#pragma once
#include "Camera.h"
#include "glm/gtx/string_cast.hpp"


constexpr static int X = 0;
constexpr static int Y = 1;
constexpr static int Z = 2;

const unsigned int PLAYER_HEIGHT = 3;

const void Camera::Update(float current_terrain_y)
{
	this->DirectionUpdate();
	this->KeyUpdate();
	this->JumpUpdate(current_terrain_y);

	// Update Matrices
	glm::vec3 position_vec = glm::vec3(pos[X], pos[Y], pos[Z]);
	this->view = glm::lookAt(position_vec, position_vec + direction, glm::vec3(0, 1, 0));
	this->rotation = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0) + direction, glm::vec3(0, 1, 0));



	// Update Uniforms
	//glUniformMatrix4fv(r_u_projection, 1, false, glm::value_ptr(projection));
	//glUniformMatrix4fv(r_u_view, 1, false, glm::value_ptr(view));
	//glUniform3f(r_u_pos, pos[X], pos[Y], pos[Z]);


	/*view = glm::lookAt(glm::vec3(pos[0], pos[1], pos[2]), glm::vec3(pos[0], pos[1], pos[2]) + direction, glm::vec3(0, 1, 0));
	rotation = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0) + direction, glm::vec3(0, 1, 0));*/

};


bool estimate_landed(float floor_y, float player_y, float speed)
{
	if (speed <= 0)
	{
		if ((player_y + speed) <= (floor_y + PLAYER_HEIGHT))
			return true;
		else
			return false;
	}
	else
	{
		return false;
	}
}

void Camera::JumpUpdate(float current_terrain_y)
{
	if (this->jumping)
	{
		float new_y = this->pos[Y] += this->jump_speed;

		if (estimate_landed(current_terrain_y, this->pos[Y], this->jump_speed))
		{
			this->jumping = false;
			this->jump_speed = DEFAULT_JUMP_SPEED;
			this->pos[Y] = current_terrain_y + PLAYER_HEIGHT;
		}

		else
		{
			if (this->jump_speed > -6.4f)
			{
				this->jump_speed -= .03f;
			}
			this->pos[Y] = new_y;
		}
	}
	else
	{
		this->pos[Y] = current_terrain_y + PLAYER_HEIGHT;
	}

}


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

	
	this->camDir.x = cos(yaw) * cos(pitch);
	this->camDir.y = sin(pitch);
	this->camDir.z = sin(yaw) * cos(pitch);

	direction = camDir;
};

void Camera::KeyUpdate()
{
	this->movement_speed = 1;
	if (glfwGetKey(this->p_window, GLFW_KEY_LEFT_SHIFT))
		this->movement_speed = 12;
	
	auto& before_update_pos = this->pos;

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

	if (glfwGetKey(this->p_window, GLFW_KEY_Q))
	{
		
	}
	else if (glfwGetKey(this->p_window, GLFW_KEY_SPACE))
	{
		this->jumping = true;
	}


	if (glfwGetKey(this->p_window, GLFW_KEY_TAB))
	{
		std::cout		<< "X: " << this->pos[X]
						<< " Y: " << this->pos[Y]
						<< " Z: " << this->pos[Z] << std::endl;
	}

};


Camera::Camera(Window* window, int window_size[])
	: p_window(window)
	// Uniforms
	//r_u_view(shader.u_view),
	//r_u_projection(shader.u_projection),
	//r_u_pos(shader.u_cam_pos)
	// NOTE: projection Matrix is setup once

{
	projection = glm::perspective(glm::radians(60.0f), (float)(window_size[0] / window_size[1]), 0.1f, 100.0f);
	//std::cout << glm::to_string(projection) << std::endl;
	view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
	

}

