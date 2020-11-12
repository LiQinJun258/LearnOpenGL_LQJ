#include "Shader.h"
#include <iostream>
#include <fstream>
#include <SStream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	//从文件路径中获取顶点/片段着色器
	ifstream vertexFile;
	ifstream fragmentFile;
	stringstream vertexSStream;
	stringstream fragmentSStream;
	//打开文件
	vertexFile.open(vertexPath);
	fragmentFile.open(fragmentPath);
	//保证ifstream对象可以抛出异常：
	vertexFile.exceptions(ifstream::failbit || ifstream::badbit);
	fragmentFile.exceptions(ifstream::failbit || ifstream::badbit);
	
	try
	{
		if (!vertexFile.is_open() || !fragmentFile.is_open()) 
		{
			throw exception("open file error");
		}

		//读取文件缓冲内容到数据流
		vertexSStream << vertexFile.rdbuf();
		fragmentSStream << fragmentFile.rdbuf();

		//转换数据流到string
		vertexString = vertexSStream.str();
		fragmentString = fragmentSStream.str();

		vertexSource = vertexString.c_str();
		fragmentSource = fragmentString.c_str();

		// 编译着色器
		unsigned int vertex, fragment;
		// 顶点着色器
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vertexSource, NULL);
		glCompileShader(vertex);
		checkCompileErrors(vertex, "VERTEX");

		// 片段着色器
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fragmentSource, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");

		// 着色器程序
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		checkCompileErrors(ID, "PROGRAM");

		// 删除着色器，它们已经链接到我们的程序中了，已经不再需要了
		glDeleteShader(vertex);
		glDeleteShader(fragment);

	}
	catch (const std::exception& ex)
	{
		printf(ex.what());
	}
}

Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
	//从文件路径中获取顶点/片段着色器
	ifstream vertexFile;
	ifstream fragmentFile;
	ifstream geometryFile;
	stringstream vertexSStream;
	stringstream fragmentSStream;
	stringstream geometrySStream;
	//打开文件
	vertexFile.open(vertexPath);
	fragmentFile.open(fragmentPath);
	geometryFile.open(geometryPath);
	//保证ifstream对象可以抛出异常：
	vertexFile.exceptions(ifstream::failbit || ifstream::badbit);
	fragmentFile.exceptions(ifstream::failbit || ifstream::badbit);
	geometryFile.exceptions(ifstream::failbit || ifstream::badbit);

	try
	{
		if (!vertexFile.is_open() || !fragmentFile.is_open() || !geometryFile.is_open())
		{
			throw exception("open file error");
		}

		//读取文件缓冲内容到数据流
		vertexSStream << vertexFile.rdbuf();
		fragmentSStream << fragmentFile.rdbuf();
		geometrySStream << geometryFile.rdbuf();

		//转换数据流到string
		vertexString = vertexSStream.str();
		fragmentString = fragmentSStream.str();
		geometryString = geometrySStream.str();

		vertexSource = vertexString.c_str();
		fragmentSource = fragmentString.c_str();
		geometrySource = geometryString.c_str();

		// 编译着色器
		unsigned int vertex, fragment, geometry;
		// 顶点着色器
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vertexSource, NULL);
		glCompileShader(vertex);
		checkCompileErrors(vertex, "VERTEX");

		// 片段着色器
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fragmentSource, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");

		// 几何着色器
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &geometrySource, NULL);
		glCompileShader(geometry);
		checkCompileErrors(geometry, "GEOMETRY");

		// 着色器程序
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glAttachShader(ID, geometry);
		glLinkProgram(ID);
		checkCompileErrors(ID, "PROGRAM");

		// 删除着色器，它们已经链接到我们的程序中了，已经不再需要了
		glDeleteShader(vertex);
		glDeleteShader(fragment);
		glDeleteShader(geometry);
	}
	catch (const std::exception& ex)
	{
		printf(ex.what());
	}
}

void Shader::use() 
{
	glUseProgram(ID);
}

void Shader::SetUniform3f(const char * paraNameString, glm::vec3 param)
{
	glUniform3f(glGetUniformLocation(ID, paraNameString), param.x, param.y, param.z);
}

void Shader::SetUniform1f(const char * paraNameString, float param)
{
	glUniform1f(glGetUniformLocation(ID, paraNameString), param);
}

void Shader::setVec2(const std::string &name, float x, float y)
{
	glUniform2f(glGetUniformLocation(ID, name.c_str()),x,y);
}

void Shader::SetUniform1i(const char * paraNameString, int slot)
{
	glUniform1i(glGetUniformLocation(ID, paraNameString), slot);
}

void Shader::checkCompileErrors(unsigned int ID, std::string type) {
	int sucess;
	char infolog[512];

	if (type != "PROGRAM")
	{
		glGetShaderiv(ID, GL_COMPILE_STATUS, &sucess);
		if (!sucess)
		{
			glGetShaderInfoLog(ID, 512, NULL, infolog);
			cout << "shader compile error:" << infolog << endl;
		}
	}
	else
	{
		glGetProgramiv(ID, GL_LINK_STATUS, &sucess);
		if (!sucess)
		{
			glGetProgramInfoLog(ID, 512, NULL, infolog);
			cout << "program linking error:" << infolog << endl;
		}
	}
}