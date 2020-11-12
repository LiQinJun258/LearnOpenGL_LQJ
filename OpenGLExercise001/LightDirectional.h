#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

class LightDirectional
{
public:
	LightDirectional(glm::vec3 _angles, glm::vec3 _color = glm::vec3(1.0f,1.0f,1.0f));

	glm::vec3 angles;
	glm::vec3 direction=glm::vec3(0,0,1.0f);
	glm::vec3 color;

	void UpdateDiection();
};

