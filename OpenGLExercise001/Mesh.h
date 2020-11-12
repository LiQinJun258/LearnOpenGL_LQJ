#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "Shader.h"
#include <GL/glew.h>
#include <iostream>
struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh
{
public:
	/*  网格数据  */
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	/*  函数  */
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	void DrawArray(Shader* shader,int diffuse , int specular , int emission);
	void DrawSliceArray(Shader* shader, int diffuse, int specular, int emission);
	void DrawElements(Shader* shader);
	/*  测试  */
	Mesh(float vertices[]);
	/*  渲染数据  */
	unsigned int VAO, VBO, EBO; 
private:
	
	/*  函数  */
	void setupElementsMesh();
	void setupArrayMesh();
};

