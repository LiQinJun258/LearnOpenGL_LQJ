#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:
	~Camera();
	//position 相机所在位置  target 相机指向的位置  worldup 世界向上的向量
	Camera(glm::vec3 position, glm::vec3 target, glm::vec3 worldup);
	//pitch俯仰角和yaw偏航角
	Camera(glm::vec3 position, float pitch, float yaw, glm::vec3 worldup);
	
	
	//摄影机位置
	glm::vec3 Position;
	//Forward 摄影机的“方向”（一个和朝向相反的向量）
	glm::vec3 Forward;
	glm::vec3 Right;
	//摄影机的上方向
	glm::vec3 Up;
	//世界的上方向
	glm::vec3 WorldUp;
	float Pitch;
	float Yaw;
	float cameraPosSpeed;


	glm::mat4 GetViewMatrix();
	void ProcessMouseMovement(float deltaX, float deltaY);
	void PosUpdateForward();
	void PosUpdateBackward();
	void PosUpdateLeft();
	void PosUpdateRight();
	void PosUpdateUp();
	void PosUpdateDown();

private:
	void UpdateCameraVectors();
};

