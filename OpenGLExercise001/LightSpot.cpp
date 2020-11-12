#include "LightSpot.h"

LightSpot::LightSpot(glm::vec3 _position, glm::vec3 _angles, glm::vec3 _color):
	position(_position),
	angles(_angles),
	color(_color)
{
	constant = 1.0f;
	linear = 0.09f;
	quadratic = 0.032f;
	UpdateDiection();
}

void LightSpot::UpdateDiection()
{
	direction = glm::vec3(0, 0, 1.0f);//pointing to Z
	direction = glm::rotateZ(direction, glm::radians(angles.z));
	direction = glm::rotateX(direction, glm::radians(angles.x));
	direction = glm::rotateY(direction, glm::radians(angles.y));
	direction *= -1.0f;
}