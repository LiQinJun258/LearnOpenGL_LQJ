#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 worldup)
{
	Position = position;
	WorldUp = worldup;
	//normalize 归一化变成单位向量
	//Forward 摄影机的“方向”（一个和朝向相反的向量）
	Forward = glm::normalize(position - target);
	//Right 它代表Camera的右方，用世界的上方向与摄影机朝向叉乘
	Right = glm::normalize(glm::cross(WorldUp, Forward));
	//UP  摄像机的上方向
	Up = glm::cross(Forward, Right);
}

glm::mat4 Camera::GetViewMatrix()
{
	//glm::LookAt函数需要一个摄像机位置、一个目标位置和表示世界空间中的上向量的向量。
	//它会创建一个观察矩阵。
	return glm::lookAt(Position, Position - Forward, WorldUp);
}

Camera::Camera(glm::vec3 position, float pitch, float yaw, glm::vec3 worldup)
{
	Position = position;
	WorldUp = worldup;
	Pitch = pitch;
	Yaw = yaw;
	Forward.x = cos(glm::radians(Pitch)) * sin(glm::radians(Yaw));
	Forward.y = sin(glm::radians(Pitch));
	Forward.z = cos(glm::radians(Pitch)) * cos(glm::radians(Yaw));
	Forward = glm::normalize(Forward);
	//Right 它代表摄像机空间的x轴的正方向
	Right = glm::normalize(glm::cross(WorldUp, Forward));
	//UP 一个指向摄像机的正y轴向量
	Up = glm::cross(Forward, Right);
}

void Camera::ProcessMouseMovement(float deltaX, float deltaY)
{
	Pitch += deltaY;
	Yaw -= deltaX;

	UpdateCameraVectors();
	if (Pitch > 89.0f)
		Pitch = 89.0f;
	if (Pitch < -89.0f)
		Pitch = -89.0f;
}

//-Forward是当前相机的朝向
//朝前
void Camera::PosUpdateForward()
{
	Position += cameraPosSpeed * -Forward;
}
//朝后
void Camera::PosUpdateBackward()
{
	Position -= cameraPosSpeed * -Forward;
}
//朝上
void Camera::PosUpdateUp()
{
	Position += cameraPosSpeed * WorldUp;
}
//朝下
void Camera::PosUpdateDown()
{
	Position -= cameraPosSpeed * WorldUp;
}
//左边
void Camera::PosUpdateLeft()
{
	Position -= glm::normalize(glm::cross(-Forward, Up)) * cameraPosSpeed;
}
//右边
void Camera::PosUpdateRight()
{
	Position += glm::normalize(glm::cross(-Forward, Up)) * cameraPosSpeed;
}


void Camera::UpdateCameraVectors()
{
	Forward.x = cos(glm::radians(Pitch)) * sin(glm::radians(Yaw));
	Forward.y = sin(glm::radians(Pitch));
	Forward.z = cos(glm::radians(Pitch)) * cos(glm::radians(Yaw));
	Forward = glm::normalize(Forward);
	//Right 它代表摄像机空间的x轴的正方向
	Right = glm::normalize(glm::cross(WorldUp, Forward));
	//UP 一个指向摄像机的正y轴向量
	Up = glm::cross(Forward, Right);
}


Camera::~Camera()
{
}