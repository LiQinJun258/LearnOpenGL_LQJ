#include "LightDirectional.h"

LightDirectional::LightDirectional(glm::vec3 _angles, glm::vec3 _color):
	angles(_angles),
	color(_color)
{
	UpdateDiection();
}

void LightDirectional::UpdateDiection() {
	direction = glm::vec3(0, 0, 1.0f);//这是光线方向
	direction = glm::rotateZ(direction, glm::radians(angles.z));
	direction = glm::rotateX(direction, glm::radians(angles.x));
	direction = glm::rotateY(direction, glm::radians(angles.y));
	//这是指向光的方向（与光线相反）
	direction *= -1.0f;
}