#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Camera.h"
#include "Material.h"
#include "LightDirectional.h"
#include "LightPoint.h"
#include "LightSpot.h"
#include "Mesh.h"
#include "Model.h"


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



#pragma region Model Data
//正方体的数据
float vertices[] = {
	//position             //normal             //TexCoord 
	-0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,	0.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,	1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,	1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,	1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,	0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,	0.0f, 1.0f,

	-0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,	0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,	1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,	1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,	1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,	0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,	0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,	1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,	1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,	1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,	1.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f,	1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,	1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f,	0.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,	0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,	1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,	1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,	1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,	0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,	0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,	0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,	1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,	1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,	1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,	0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,	0.0f, 0.0f,

};

glm::vec3 cubePositions[] = {
  glm::vec3(0.0f,  0.0f,  0.0f),
  glm::vec3(2.0f,  5.0f, -15.0f),
  glm::vec3(-1.5f, -2.2f, -2.5f),
  glm::vec3(-3.8f, -2.0f, -12.3f),
  glm::vec3(2.4f, -0.4f, -3.5f),
  glm::vec3(-1.7f,  3.0f, -7.5f),
  glm::vec3(1.3f, -2.0f, -2.5f),
  glm::vec3(1.5f,  2.0f, -2.5f),
  glm::vec3(1.5f,  0.2f, -1.5f),
  glm::vec3(-1.3f,  1.0f, -1.5f)
};

//草与玻璃的数据（都是一片）
float grassVertices[] = {
	//position             //normal             //TexCoord 
	-0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,	0.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,	1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,	1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,	1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,	0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,	0.0f, 1.0f,
};
glm::vec3 grassPosition[] = {
	glm::vec3(-1.5f,  0.0f, 1.0f),
	glm::vec3(1.5f,  0.0f, 1.0f),
};
glm::vec3 glassPosition[] = {
	glm::vec3(-1.5f,  1.0f, 2.0f),
	glm::vec3(-1.2f,  0.9f, 1.5f),
	glm::vec3(0.5f,  1.5f, 1.0f),
};

//地板的数据
float planeVertices[] = {
	// positions            // normals         // texcoords
	 -35.0f, 0.5f ,-35.0f,  0.0f, 1.0f, 0.0f,   0.0f,  35.0f,
	 35.0f, 0.5f, 35.0f,  0.0f, 1.0f, 0.0f,  35.0f, 0.0f,
	 35.0f, 0.5f, -35.0f,  0.0f, 1.0f, 0.0f,   35.0f, 35.0f,
			 
	 35.0f, 0.5f , 35.0f,  0.0f, 1.0f, 0.0f,  35.0f, 0.0f,
	-35.0f, 0.5f ,-35.0f,  0.0f, 1.0f, 0.0f,   0.0f, 35.0f,
	-35.0f, 0.5f ,35.0f,  0.0f, 1.0f, 0.0f,  0.0f ,0.0f,
};


float quadVertices[] = {

	// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f

};

float skyboxVertices[] = {
	// positions          
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
};


//skyBoxReflection
float skyBoxReflectionVertices[] = {
	//position             //normal           
	-0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,
	-0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,

	-0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,
	-0.5f,   0.5f, 0.5f,   0.0f,  1.0f, 0.0f,
};
#pragma endregion

#pragma region Camera Declare
//建立camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
Camera camera(cameraPos, cameraTarget, cameraUp);
#pragma endregion

#pragma region Light Declare
//position angle color
LightDirectional lightD(
	glm::vec3(135.0f, 0, 0),
	glm::vec3(0.9f, 0.9f, 0.9f));
glm::vec3 lightPos(0.0f, 10.0f, 10.0f);

//LightPoint lightP0(glm::vec3(1.0f, 0, 0),
//	glm::vec3(1.0f, 0, 0));
//
//LightPoint lightP1(glm::vec3(0, 1.0f, 0),
//	glm::vec3(0, 1.0f, 0));
//
//LightPoint lightP2(glm::vec3(0, 0, 1.0f),
//	glm::vec3(0, 0, 1.0f));
//
//LightPoint lightP3(glm::vec3(0.0f, 0.0f, -7.0f),
//	glm::vec3(1.0f, 1.0f, 1.0f));
//
//LightSpot lightS(glm::vec3(0, 8.0f, 0.0f),
//	glm::vec3(135.0f, 0, 0),
//	glm::vec3(0, 0, 1.5f));
#pragma endregion

#pragma region Input Declare
//移动用
float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间

void processInput(GLFWwindow* window) {
	//看是不是按下esc键 然后退出
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	//更流畅点的摄像机系统
	if (deltaTime != 0) {
		camera.cameraPosSpeed = 5 * deltaTime;
	}
	//camera前后左右根据镜头方向移动
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.PosUpdateForward();
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.PosUpdateBackward();
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.PosUpdateLeft();
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.PosUpdateRight();
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		camera.PosUpdateUp();
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		camera.PosUpdateDown();


}
float lastX;
float lastY;
bool firstMouse = true;

//鼠标控制镜头方向
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse == true)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	float deltaX, deltaY;
	float sensitivity = 0.05f;

	deltaX = (xpos - lastX)*sensitivity;
	deltaY = (ypos - lastY)*sensitivity;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(deltaX, deltaY);

};
//缩放
float fov = 45.0f;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= yoffset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	//确保视口与新的窗口尺寸匹配；注意宽度和高度将大于屏幕显示上指定的尺寸
	glViewport(0, 0, width, height);
}
#pragma endregion

//加载一般的图片
unsigned int LoadImageToGPU(const char* filename, GLint internalFormat, GLenum format) {
	unsigned int TexBuffer;
	glGenTextures(1, &TexBuffer);
	glBindTexture(GL_TEXTURE_2D, TexBuffer);

	// 为当前绑定的纹理对象设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	// 加载并生成纹理
	int width, height, nrChannel;
	unsigned char *data = stbi_load(filename, &width, &height, &nrChannel, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		printf("Failed to load texture");
	}
	stbi_image_free(data);
	return TexBuffer;
}


//加载天空盒图片
unsigned int loadCubemap(std::vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_SRGB_ALPHA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}



int main() {

#pragma region Open a Window
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Open GLFW Window
	GLFWwindow* window = glfwCreateWindow(800, 600, "My OpenGL Game", NULL, NULL);
	if (window == NULL)
	{
		printf("Open window failed.");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	//关闭鼠标显示
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//回调函数监听鼠标
	glfwSetCursorPosCallback(window, mouse_callback);
	//回调函数监听滚轮
	glfwSetScrollCallback(window, scroll_callback);
	// 每当窗口改变大小就会启动
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	//Init GLEW
	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		printf("Init GLEW failed.");
		glfwTerminate();
		return -1;
	}


	glViewport(0, 0, 800, 600);
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);

	//glEnable(GL_STENCIL_TEST);
	//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//
	//glEnable(GL_CULL_FACE);
#pragma endregion

#pragma region Init Shader Program
	Shader* myShader = new Shader("vertexSource.vert", "fragmentSource.frag");
	Shader* myShaderObj = new Shader("vertexSourceObjExplode.vert", "fragmentSource.frag", "geometrySource.geom");

	Shader* ShaderNormal = new Shader("vertexSourceNormal.vert", "fragmentSourceNormal.frag", "geometrySourceNormal.geom");

	Shader* border = new Shader("vertexSource.vert", "Border.frag");
	Shader* grass = new Shader("vertexSource.vert", "grass.frag");
	Shader* glass = new Shader("vertexSource.vert", "glass.frag");



	Shader* screenShader = new Shader("screen.vert", "screen.frag");
	Shader* skyBox = new Shader("skyBox.vert", "skyBox.frag");

	Shader* skyBoxReflection = new Shader("skyBoxReflection.vert", "skyBoxReflection.frag");


	


#pragma endregion

#pragma region FBO / RBO
	//配置MSAA帧缓冲区
	unsigned int fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	//创建多采样颜色附加纹理
	unsigned int textureColorBufferMultiSampled;
	glGenTextures(1, &textureColorBufferMultiSampled);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, 800, 600, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled, 0);


	//为深度和模板附件创建（也是多采样）renderbuffer对象
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, 800, 600);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	// 将它附加当前绑定的帧缓冲对象
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);


	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// 配置第二个正常的中介帧缓冲
	unsigned int intermediateFBO;
	glGenFramebuffers(1, &intermediateFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, intermediateFBO);

	// 创建颜色附加纹理
	unsigned int screenTexture;
	glGenTextures(1, &screenTexture);
	glBindTexture(GL_TEXTURE_2D, screenTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTexture, 0);	// we only need a color buffer

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Intermediate framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);




	// lighting info
	// -------------
	glm::vec3 lightPos(0.0f, 10.0f, 10.0f);

#pragma endregion


#pragma region Init Material
	Material * myMaterial = new Material(myShader,
		LoadImageToGPU("container2.png", GL_SRGB_ALPHA, GL_RGBA),
		LoadImageToGPU("container2_specular.png", GL_RGBA, GL_RGBA),
		LoadImageToGPU("matrix.jpg", GL_RGB, GL_RGB),
		glm::vec3(1.0f, 1.0f, 1.0f),
		150.0f);

	Material * myMaterialNormal = new Material(ShaderNormal,
		LoadImageToGPU("container2.png", GL_SRGB_ALPHA, GL_RGBA),
		LoadImageToGPU("container2_specular.png", GL_RGBA, GL_RGBA),
		LoadImageToGPU("matrix.jpg", GL_RGB, GL_RGB),
		glm::vec3(1.0f, 1.0f, 1.0f),
		150.0f);

	Material * Grass = new Material(grass,
		LoadImageToGPU("grass.png", GL_SRGB_ALPHA, GL_RGBA),
		LoadImageToGPU("grass.png", GL_RGBA, GL_RGBA),
		LoadImageToGPU("grass.png", GL_RGBA, GL_RGBA),
		glm::vec3(1.0f, 1.0f, 1.0f),
		150.0f);

	Material * Glass = new Material(glass,
		LoadImageToGPU("blending_transparent_window.png", GL_SRGB_ALPHA, GL_RGBA),
		LoadImageToGPU("blending_transparent_window.png", GL_RGBA, GL_RGBA),
		LoadImageToGPU("blending_transparent_window.png", GL_RGBA, GL_RGBA),
		glm::vec3(1.0f, 1.0f, 1.0f),
		150.0f);

	Material * Plane = new Material(myShader,
		LoadImageToGPU("wood.jpg", GL_SRGB, GL_RGB),
		LoadImageToGPU("wood.jpg", GL_RGB, GL_RGB),
		LoadImageToGPU("wood.jpg", GL_RGB, GL_RGB),
		glm::vec3(1.0f, 1.0f, 1.0f),
		150.0f);
	std::vector<std::string> faces
	{
		"right.jpg",
		"left.jpg",
		"top.jpg",
		"bottom.jpg",
		"front.jpg",
		"back.jpg"
	};
	unsigned int cubemapTexture = loadCubemap(faces);

#pragma endregion

#pragma region Init and Load Models to VAO,VBO
	Mesh cube(vertices);
	Mesh grassMesh(grassVertices);
	Mesh glassMesh(grassVertices);
	Mesh planeMesh(planeVertices);
	std::string exePath = "..\\model\\nanosuit.obj";
	Model model(exePath);


	// screen quad VAO
	unsigned int quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));


	// skyBox VAO
	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


	//skyBoxReflection VAO
	unsigned int skyBoxReflectionVAO, skyBoxReflectionVBO;
	glGenVertexArrays(1, &skyBoxReflectionVAO);
	glGenBuffers(1, &skyBoxReflectionVBO);
	glBindVertexArray(skyBoxReflectionVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyBoxReflectionVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyBoxReflectionVertices), &skyBoxReflectionVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	
	
	//坐标翻转
	stbi_set_flip_vertically_on_load(true);
#pragma endregion



#pragma region Prepare MVP matrices
	//model
	glm::mat4 modelMat;
	//view
	glm::mat4 viewMat;
	//projection
	glm::mat4 projMat;

	//设置观察矩阵和投影矩阵的uniform缓冲
	unsigned int uniformBlockIndexmyShader = glGetUniformBlockIndex(myShader->ID, "Matrices");
	unsigned int uniformBlockIndexShaderNormal = glGetUniformBlockIndex(ShaderNormal->ID, "Matrices");
	unsigned int uniformBlockIndexmyShaderObj = glGetUniformBlockIndex(myShaderObj->ID, "Matrices");
	unsigned int uniformBlockIndexborder = glGetUniformBlockIndex(border->ID, "Matrices");
	unsigned int uniformBlockIndexgrass = glGetUniformBlockIndex(grass->ID, "Matrices");
	unsigned int uniformBlockIndexglass = glGetUniformBlockIndex(glass->ID, "Matrices");
	unsigned int uniformBlockIndexskyBoxReflection = glGetUniformBlockIndex(skyBoxReflection->ID, "Matrices");

	glUniformBlockBinding(myShader->ID, uniformBlockIndexmyShader, 0);
	glUniformBlockBinding(ShaderNormal->ID, uniformBlockIndexShaderNormal, 0);
	glUniformBlockBinding(myShaderObj->ID, uniformBlockIndexmyShaderObj, 0);
	glUniformBlockBinding(border->ID, uniformBlockIndexborder, 0);
	glUniformBlockBinding(grass->ID, uniformBlockIndexgrass, 0);
	glUniformBlockBinding(glass->ID, uniformBlockIndexglass, 0);
	glUniformBlockBinding(skyBoxReflection->ID, uniformBlockIndexskyBoxReflection, 0);

	//创建Uniform缓冲对象本身，并将其绑定到绑定点0
	unsigned int uboMatrices;
	glGenBuffers(1, &uboMatrices);

	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4));

	

#pragma endregion


	while (!glfwWindowShouldClose(window))
	{
		//Process Input
		processInput(window);

		//第一阶段处理，渲染到自己建立的fbo
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		

		#pragma region Render



		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_CULL_FACE);



		//Clear Screen
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		//设置好uniform缓冲中的观察矩阵
		viewMat = camera.GetViewMatrix();
		glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(viewMat));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		//设置好透视矩阵
		projMat = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);
		glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projMat));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);


		//炸裂机器人绘制
		modelMat = glm::translate(glm::mat4(1.0f), { 0,-10,-5 });

		myShaderObj->use();
		//机器人的uniform
		glUniformMatrix4fv(glGetUniformLocation(myShaderObj->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat));

		//glUniformMatrix4fv(glGetUniformLocation(myShaderObj->ID, "viewMat"), 1, GL_FALSE, glm::value_ptr(viewMat));
		//glUniformMatrix4fv(glGetUniformLocation(myShaderObj->ID, "projMat"), 1, GL_FALSE, glm::value_ptr(projMat));

		glUniform3f(glGetUniformLocation(myShaderObj->ID, "cameraPos"), camera.Position.x, camera.Position.y, camera.Position.z);

		glUniform1f(glGetUniformLocation(myShaderObj->ID, "time"), glfwGetTime());
		glUniform1f(glGetUniformLocation(myShaderObj->ID, "time1"), glfwGetTime());

		glUniform3f(glGetUniformLocation(myShaderObj->ID, "objColor"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(myShaderObj->ID, "ambientColor"), 0.1f, 0.1f, 0.1f);

		glUniform3f(glGetUniformLocation(myShaderObj->ID, "lightD.color"), lightD.color.x, lightD.color.y, lightD.color.z);
		glUniform3f(glGetUniformLocation(myShaderObj->ID, "lightD.dirToLight"), lightD.direction.x, lightD.direction.y, lightD.direction.z);


		//glUniform3f(glGetUniformLocation(myShaderObj->ID, "lightP0.pos"), lightP0.position.x, lightP0.position.y, lightP0.position.z);
		//glUniform3f(glGetUniformLocation(myShaderObj->ID, "lightP0.color"), lightP0.color.x, lightP0.color.y, lightP0.color.z);
		//glUniform1f(glGetUniformLocation(myShaderObj->ID, "lightP0.constant"), lightP0.constant);
		//glUniform1f(glGetUniformLocation(myShaderObj->ID, "lightP0.linear"), lightP0.linear);
		//glUniform1f(glGetUniformLocation(myShaderObj->ID, "lightP0.quadratic"), lightP0.quadratic);

		//glUniform3f(glGetUniformLocation(myShaderObj->ID, "lightP1.pos"), lightP1.position.x, lightP1.position.y, lightP1.position.z);
		//glUniform3f(glGetUniformLocation(myShaderObj->ID, "lightP1.color"), lightP1.color.x, lightP1.color.y, lightP1.color.z);
		//glUniform1f(glGetUniformLocation(myShaderObj->ID, "lightP1.constant"), lightP1.constant);
		//glUniform1f(glGetUniformLocation(myShaderObj->ID, "lightP1.linear"), lightP1.linear);
		//glUniform1f(glGetUniformLocation(myShaderObj->ID, "lightP1.quadratic"), lightP1.quadratic);

		//glUniform3f(glGetUniformLocation(myShaderObj->ID, "lightP2.pos"), lightP2.position.x, lightP2.position.y, lightP2.position.z);
		//glUniform3f(glGetUniformLocation(myShaderObj->ID, "lightP2.color"), lightP2.color.x, lightP2.color.y, lightP2.color.z);
		//glUniform1f(glGetUniformLocation(myShaderObj->ID, "lightP2.constant"), lightP2.constant);
		//glUniform1f(glGetUniformLocation(myShaderObj->ID, "lightP2.linear"), lightP2.linear);
		//glUniform1f(glGetUniformLocation(myShaderObj->ID, "lightP2.quadratic"), lightP2.quadratic);

		//glUniform3f(glGetUniformLocation(myShaderObj->ID, "lightP3.pos"), lightP3.position.x, lightP3.position.y, lightP3.position.z);
		//glUniform3f(glGetUniformLocation(myShaderObj->ID, "lightP3.color"), lightP3.color.x, lightP3.color.y, lightP3.color.z);
		//glUniform1f(glGetUniformLocation(myShaderObj->ID, "lightP3.constant"), lightP3.constant);
		//glUniform1f(glGetUniformLocation(myShaderObj->ID, "lightP3.linear"), lightP3.linear);
		//glUniform1f(glGetUniformLocation(myShaderObj->ID, "lightP3.quadratic"), lightP3.quadratic);

		//glUniform3f(glGetUniformLocation(myShaderObj->ID, "lightS.pos"), lightS.position.x, lightS.position.y, lightS.position.z);
		//glUniform3f(glGetUniformLocation(myShaderObj->ID, "lightS.color"), lightS.color.x, lightS.color.y, lightS.color.z);
		//glUniform3f(glGetUniformLocation(myShaderObj->ID, "lightS.dirToLight"), lightS.direction.x, lightS.direction.y, lightS.direction.z);
		//glUniform1f(glGetUniformLocation(myShaderObj->ID, "lightS.constant"), lightS.constant);
		//glUniform1f(glGetUniformLocation(myShaderObj->ID, "lightS.linear"), lightS.linear);
		//glUniform1f(glGetUniformLocation(myShaderObj->ID, "lightS.quadratic"), lightS.quadratic);
		//glUniform1f(glGetUniformLocation(myShaderObj->ID, "lightS.cosPhyInner"), lightS.cosPhyInner);
		//glUniform1f(glGetUniformLocation(myShaderObj->ID, "lightS.cosPhyOuter"), lightS.cosPhyOuter);


		glUniform3f(glGetUniformLocation(myShaderObj->ID, "material.ambient"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(myShaderObj->ID, "material.shininess"), 150.0f);

		glStencilMask(0x00); // 记得保证我们在绘制机器人的时候不会更新模板缓冲
		glUniform1f(glGetUniformLocation(myShaderObj->ID, "What"), 1);
		//注释掉的为反射机器人
		//skyBoxReflection->use();
		//model.Draw(skyBoxReflection);
		glDisable(GL_CULL_FACE);
		model.Draw(myShaderObj);




		glEnable(GL_CULL_FACE);

		//折射机器人
		skyBoxReflection->use();
		modelMat = glm::translate(glm::mat4(1.0f), { 0,-10,-25 });
		//Set view matrix
		//viewMat = camera.GetViewMatrix();

		//Set projection matrix
		//projMat = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);

		glUniformMatrix4fv(glGetUniformLocation(skyBoxReflection->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
		//glUniformMatrix4fv(glGetUniformLocation(skyBoxReflection->ID, "viewMat"), 1, GL_FALSE, glm::value_ptr(viewMat));
		//glUniformMatrix4fv(glGetUniformLocation(skyBoxReflection->ID, "projMat"), 1, GL_FALSE, glm::value_ptr(projMat));
		model.Draw(skyBoxReflection);



		//法向量机器人
		ShaderNormal->use();
		modelMat = glm::translate(glm::mat4(1.0f), { 0,-10,-15 });
		glUniformMatrix4fv(glGetUniformLocation(ShaderNormal->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
		model.Draw(ShaderNormal);
		glDisable(GL_CULL_FACE);


		//画草
		for (unsigned int i = 0; i < 2; i++)
		{
			//Set Model matrix
			modelMat = glm::translate(glm::mat4(1.0f), grassPosition[i]);
			modelMat = glm::rotate(modelMat, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			//Set view matrix
			//viewMat = camera.GetViewMatrix();

			//Set projection matrix
			//projMat = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);
			//Set Material -> Shader Program
			grass->use();
			glUniformMatrix4fv(glGetUniformLocation(grass->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
			//glUniformMatrix4fv(glGetUniformLocation(grass->ID, "viewMat"), 1, GL_FALSE, glm::value_ptr(viewMat));
			//glUniformMatrix4fv(glGetUniformLocation(grass->ID, "projMat"), 1, GL_FALSE, glm::value_ptr(projMat));


			grassMesh.DrawSliceArray(Grass->shader, Grass->diffuse, Grass->specular, Grass->emission);
		}

		glEnable(GL_CULL_FACE);



		//方块  
		for (unsigned int i = 0; i < 10; i++)
		{
			myShader->use();

			//Set Material -> Uniforms

			//glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "viewMat"), 1, GL_FALSE, glm::value_ptr(viewMat));
			//glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "projMat"), 1, GL_FALSE, glm::value_ptr(projMat));

			glUniform3f(glGetUniformLocation(myShader->ID, "cameraPos"), camera.Position.x, camera.Position.y, camera.Position.z);

			glUniform1f(glGetUniformLocation(myShader->ID, "time"), glfwGetTime());

			glUniform3f(glGetUniformLocation(myShader->ID, "objColor"), 1.0f, 1.0f, 1.0f);
			glUniform3f(glGetUniformLocation(myShader->ID, "ambientColor"), 0.1f, 0.1f, 0.1f);

			glUniform3f(glGetUniformLocation(myShader->ID, "lightD.color"), lightD.color.x, lightD.color.y, lightD.color.z);
			glUniform3f(glGetUniformLocation(myShader->ID, "lightD.dirToLight"), lightD.direction.x, lightD.direction.y, lightD.direction.z);


			//glUniform3f(glGetUniformLocation(myShader->ID, "lightP0.pos"), lightP0.position.x, lightP0.position.y, lightP0.position.z);
			//glUniform3f(glGetUniformLocation(myShader->ID, "lightP0.color"), lightP0.color.x, lightP0.color.y, lightP0.color.z);
			//glUniform1f(glGetUniformLocation(myShader->ID, "lightP0.constant"), lightP0.constant);
			//glUniform1f(glGetUniformLocation(myShader->ID, "lightP0.linear"), lightP0.linear);
			//glUniform1f(glGetUniformLocation(myShader->ID, "lightP0.quadratic"), lightP0.quadratic);

			//glUniform3f(glGetUniformLocation(myShader->ID, "lightP1.pos"), lightP1.position.x, lightP1.position.y, lightP1.position.z);
			//glUniform3f(glGetUniformLocation(myShader->ID, "lightP1.color"), lightP1.color.x, lightP1.color.y, lightP1.color.z);
			//glUniform1f(glGetUniformLocation(myShader->ID, "lightP1.constant"), lightP1.constant);
			//glUniform1f(glGetUniformLocation(myShader->ID, "lightP1.linear"), lightP1.linear);
			//glUniform1f(glGetUniformLocation(myShader->ID, "lightP1.quadratic"), lightP1.quadratic);

			//glUniform3f(glGetUniformLocation(myShader->ID, "lightP2.pos"), lightP2.position.x, lightP2.position.y, lightP2.position.z);
			//glUniform3f(glGetUniformLocation(myShader->ID, "lightP2.color"), lightP2.color.x, lightP2.color.y, lightP2.color.z);
			//glUniform1f(glGetUniformLocation(myShader->ID, "lightP2.constant"), lightP2.constant);
			//glUniform1f(glGetUniformLocation(myShader->ID, "lightP2.linear"), lightP2.linear);
			//glUniform1f(glGetUniformLocation(myShader->ID, "lightP2.quadratic"), lightP2.quadratic);

			//glUniform3f(glGetUniformLocation(myShader->ID, "lightP3.pos"), lightP3.position.x, lightP3.position.y, lightP3.position.z);
			//glUniform3f(glGetUniformLocation(myShader->ID, "lightP3.color"), lightP3.color.x, lightP3.color.y, lightP3.color.z);
			//glUniform1f(glGetUniformLocation(myShader->ID, "lightP3.constant"), lightP3.constant);
			//glUniform1f(glGetUniformLocation(myShader->ID, "lightP3.linear"), lightP3.linear);
			//glUniform1f(glGetUniformLocation(myShader->ID, "lightP3.quadratic"), lightP3.quadratic);

			//glUniform3f(glGetUniformLocation(myShader->ID, "lightS.pos"), lightS.position.x, lightS.position.y, lightS.position.z);
			//glUniform3f(glGetUniformLocation(myShader->ID, "lightS.color"), lightS.color.x, lightS.color.y, lightS.color.z);
			//glUniform3f(glGetUniformLocation(myShader->ID, "lightS.dirToLight"), lightS.direction.x, lightS.direction.y, lightS.direction.z);
			//glUniform1f(glGetUniformLocation(myShader->ID, "lightS.constant"), lightS.constant);
			//glUniform1f(glGetUniformLocation(myShader->ID, "lightS.linear"), lightS.linear);
			//glUniform1f(glGetUniformLocation(myShader->ID, "lightS.quadratic"), lightS.quadratic);
			//glUniform1f(glGetUniformLocation(myShader->ID, "lightS.cosPhyInner"), lightS.cosPhyInner);
			//glUniform1f(glGetUniformLocation(myShader->ID, "lightS.cosPhyOuter"), lightS.cosPhyOuter);

			myMaterial->shader->SetUniform3f("material.ambient", myMaterial->ambient);
			myMaterial->shader->SetUniform1f("material.shininess", myMaterial->shininess);


			//模板测试绘制边框

			////方块与方块边框
			//glStencilFunc(GL_ALWAYS, 1, 0xFF); // 更新模板缓冲函数，所有的片段都要写入模板
			//glStencilMask(0xFF); // 启用模板缓冲写入
			////正常绘制十个正方体，而后记录模板值


			
			//Set Model matrix
			modelMat = glm::translate(glm::mat4(1.0f), cubePositions[i]);
			float angle = 20.0f * (i);
			//float angle = 20.0f * i + 50*glfwGetTime();

			modelMat = glm::rotate(modelMat, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

			//Set view matrix
			//viewMat = camera.GetViewMatrix();

			//Set projection matrix
			//projMat = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);


			//Set Material -> Shader Program
			/*glUniform1f(glGetUniformLocation(myShader->ID, "What"), 0);*/
			//盒子的uniform
			glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat));

			glUniform1f(glGetUniformLocation(myShader->ID, "What"), 0);
			cube.DrawArray(myMaterial->shader, myMaterial->diffuse, myMaterial->specular, myMaterial->emission);
		/*	ShaderNormal->use();
			glUniformMatrix4fv(glGetUniformLocation(ShaderNormal->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
			cube.DrawArray(myMaterialNormal->shader, myMaterialNormal->diffuse, myMaterialNormal->specular, myMaterialNormal->emission);*/

			////现在模板缓冲在箱子被绘制的地方都更新为1了,我们将要绘制放大的箱子,也就是绘制边框
			//glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			//glStencilMask(0x00); // 禁止模板缓冲的写入
			//
			//					 
			//

			////glDisable(GL_DEPTH_TEST);
			//border->use();
			//
			////Set Model matrix
			//modelMat = glm::translate(glm::mat4(1.0f), cubePositions[i-1]);
			//float angle = 20.0f * (i-1);
			//modelMat = glm::rotate(modelMat, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			//modelMat = glm::scale(modelMat, glm::vec3(1.2, 1.2, 1.2));

			//glUniformMatrix4fv(glGetUniformLocation(border->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
			//glUniformMatrix4fv(glGetUniformLocation(border->ID, "viewMat"), 1, GL_FALSE, glm::value_ptr(viewMat));
			//glUniformMatrix4fv(glGetUniformLocation(border->ID, "projMat"), 1, GL_FALSE, glm::value_ptr(projMat));
			//
			////因为之前设置了GL_NOTEQUAL，它会保证我们只绘制箱子上模板值不为1的部分
			//cube.DrawArray(border,1,1,1);
			//glStencilMask(0xFF);
			//   //glEnable(GL_DEPTH_TEST);  
			



		}

		
		glUniform1f(glGetUniformLocation(myShader->ID, "What"), 1);
		modelMat = glm::translate(glm::mat4(1.0f), { 0,-10,-15 });
		glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
		model.Draw(myShader);

		modelMat = glm::translate(glm::mat4(1.0f), { 0,-10,-20 });
		glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
		model.Draw(myShader);

		glDisable(GL_CULL_FACE);
		modelMat = glm::translate(glm::mat4(1.0f), { 0,-15,-10 });
		glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
		planeMesh.DrawSliceArray(Plane->shader, Plane->diffuse, Plane->specular, Plane->emission);
		glEnable(GL_CULL_FACE);

		//绘制反射盒子
		skyBoxReflection->use();

		modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 5.0f));

		//Set view matrix
		//viewMat = camera.GetViewMatrix();

		//Set projection matrix
		//projMat = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);

		glUniformMatrix4fv(glGetUniformLocation(skyBoxReflection->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
		//glUniformMatrix4fv(glGetUniformLocation(skyBoxReflection->ID, "viewMat"), 1, GL_FALSE, glm::value_ptr(viewMat));
		//glUniformMatrix4fv(glGetUniformLocation(skyBoxReflection->ID, "projMat"), 1, GL_FALSE, glm::value_ptr(projMat));
		glUniform3f(glGetUniformLocation(skyBoxReflection->ID, "cameraPos"), camera.Position.x, camera.Position.y, camera.Position.z);

		glBindVertexArray(skyBoxReflectionVAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);







		//最后（透明物体之前）画天空盒
		glDisable(GL_CULL_FACE);
		glDepthMask(GL_FALSE);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		skyBox->use();

		//Set view matrix
		viewMat = glm::mat4(glm::mat3(camera.GetViewMatrix()));

		//Set projection matrix
		projMat = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);
		glUniformMatrix4fv(glGetUniformLocation(skyBox->ID, "viewMat"), 1, GL_FALSE, glm::value_ptr(viewMat));
		glUniformMatrix4fv(glGetUniformLocation(skyBox->ID, "projMat"), 1, GL_FALSE, glm::value_ptr(projMat));
		glBindVertexArray(skyboxVAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);




		//画玻璃
		//利用map排序

		std::map<float, glm::vec3 > sorted;
		int length = sizeof(glassPosition) / sizeof(glassPosition[0]);
		for (unsigned int i = 0; i < length; i++)
		{
			float distance = glm::length(camera.Position - glassPosition[i]);
			sorted[distance] = glassPosition[i];
		}
		//在渲染的时候，我们将以逆序（从远到近）从map中获取值
		//使用了map的一个反向迭代器(Reverse Iterator)，反向遍历其中的条目，second会得到value，讲每个窗户四边形位移到对应的窗户位置上。
		for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
		{


			//Set Model matrix
			modelMat = glm::translate(glm::mat4(1.0f), it->second);
			modelMat = glm::rotate(modelMat, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			//Set view matrix
			//viewMat = camera.GetViewMatrix();

			//Set projection matrix
			//projMat = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);
			//Set Material -> Shader Program
			glass->use();
			glUniformMatrix4fv(glGetUniformLocation(glass->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
			//glUniformMatrix4fv(glGetUniformLocation(glass->ID, "viewMat"), 1, GL_FALSE, glm::value_ptr(viewMat));
			//glUniformMatrix4fv(glGetUniformLocation(glass->ID, "projMat"), 1, GL_FALSE, glm::value_ptr(projMat));


			grassMesh.DrawSliceArray(Glass->shader, Glass->diffuse, Glass->specular, Glass->emission);
		}





		glEnable(GL_CULL_FACE);

#pragma endregion 




		//第二阶段 渲染到默认的帧缓冲 绘制四边形
		//glBindFramebuffer(GL_FRAMEBUFFER, 0); // 返回默认
		//线框模式
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		//第二阶段 位块传送(Blit)到默认的帧缓冲中
		// 将多重采样缓冲还原到中介FBO上

		glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediateFBO);
		glBlitFramebuffer(0, 0, 800, 600, 0, 0, 800, 600, GL_COLOR_BUFFER_BIT, GL_NEAREST);


		// 现在场景是一个2D纹理缓冲，可以将这个图像用来后期处理	
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glClearColor(1.0f, 1.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		screenShader->use();
		glBindVertexArray(quadVAO);

		glBindTexture(GL_TEXTURE_2D, screenTexture);	// use the color attachment texture as the texture of the quad plane

		glDisable(GL_DEPTH_TEST);


		glUniform1i(glGetUniformLocation(screenShader->ID, "screenTexture"), 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);


		//Clean up prepare for next render loop
		glfwSwapBuffers(window);
		glfwPollEvents();

		//Recording the time
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	}
	//Exit program
	glfwTerminate();
	return 0;

}


