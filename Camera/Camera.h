#pragma once

#include <glad\glad.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.05f;
const float FOV = 45.0f;

class Camera
{
public:
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	float Yaw;
	float Pitch;

	float MovementSpeed;
	float Sensitivity;
	float Fov;

	Camera() :Position(0.0f, 0.0f, 3.0f), Front(0.0f, 0.0f, -1.0f), WorldUp(0.0f, 1.0f, 0.0f), Yaw(YAW), Pitch(PITCH), MovementSpeed(SPEED),
		Sensitivity(SENSITIVITY), Fov(FOV)
	{
		update_camera_dire();
	}

	Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up, float yaw = YAW, float pitch = PITCH, float movementSpeed = SPEED,
		float sensitivity = SENSITIVITY, float fov = FOV)
		:Position(position), Front(front), WorldUp(up), Yaw(yaw), Pitch(pitch), MovementSpeed(movementSpeed), Sensitivity(sensitivity), Fov(fov)
	{
		update_camera_dire();
	}

	Camera(float posX, float posY, float posZ, float frontX, float frontY, float frontZ, float upX, float upY, float upZ,
		float yaw = YAW, float pitch = PITCH, float movementSpeed = SPEED, float sensitivity = SENSITIVITY, float fov = FOV)
		:Position(posX, posY, posZ), Front(frontX, frontY, frontZ), WorldUp(upX, upY, upZ), Yaw(yaw), Pitch(pitch), MovementSpeed(movementSpeed),
		Sensitivity(sensitivity), Fov(fov)
	{
		update_camera_dire();
	}

	void process_keyboard(Camera_Movement cmove, float delta_time)
	{
		float speed = delta_time * MovementSpeed;
		if (cmove == FORWARD)
			Position += Front * speed;
		else if (cmove == BACKWARD)
			Position -= Front * speed;
		else if (cmove == RIGHT)
			Position += Right * speed;
		else if (cmove == LEFT)
			Position -= Right * speed;
	}

	void process_mouse_movement(float xoffset, float yoffset, bool constrainPitch = true)
	{
		Yaw += xoffset * Sensitivity;
		Pitch += yoffset * Sensitivity;

		if (constrainPitch)
		{
			if (Pitch > 90.0f)
				Pitch = 90.0f;
			else if (Pitch < -90.0f)
				Pitch = -90.0f;
		}
		update_camera_dire();
	}

	void process_scroll_movement(float yoffset)
	{
		Fov += yoffset;
		if (Fov < 1.0f)
			Fov = 1.0f;
		else if (Fov > 90.0f)
			Fov = 90.0f;
	}

	glm::mat4 getViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}

private:
	void update_camera_dire()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(Pitch))*cos(glm::radians(Yaw));
		front.y = sin(glm::radians(Pitch));
		front.z = cos(glm::radians(Pitch))*sin(glm::radians(Yaw));

		Front = glm::normalize(front);
		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}
};