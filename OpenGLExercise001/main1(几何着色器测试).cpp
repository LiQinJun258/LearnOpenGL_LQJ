#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
float points[] = {
	-0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // 左上
	 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // 右上
	 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // 右下
	-0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // 左下
};

const char* vertexShaderSource =
"#version 330 core									\n"
"layout(location = 0) in vec2 aPos;					\n"
"layout(location = 1) in vec3 aColor;				\n"
"													\n"
"out VS_OUT{										\n"
"	vec3 color;										\n"
"} vs_out;											\n"
"													\n"
"void main()										\n"
"{													\n"
"	gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);	\n"
"	vs_out.color = aColor;							\n"
"}													\n";


const char* fragmentShaderSource =
"#version 330 core									   \n"
"out vec4 FragColor;								   \n"
"in vec3 fColor;									   \n"
"void main(){										   \n"
"	FragColor = vec4(fColor, 1.0);}					   \n";

const char* geometryShaderSource =
"#version 330 core														\n"
"layout(points) in;														\n"
"layout(triangle_strip, max_vertices = 5) out;							\n"
"in VS_OUT{																\n"
"	vec3 color;															\n"
"} gs_in[];																\n"
"out vec3 fColor;														\n"
"void build_house(vec4 position)										\n"
"{																		\n"
"	fColor = gs_in[0].color;										    \n"
"	gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0);    // 1:左下	\n"
"	EmitVertex();														\n"
"	gl_Position = position + vec4(0.2, -0.2, 0.0, 0.0);    // 2:右下	\n"
"	EmitVertex();														\n"
"	gl_Position = position + vec4(-0.2, 0.2, 0.0, 0.0);    // 3:左上	\n"
"	EmitVertex();														\n"
"	gl_Position = position + vec4(0.2, 0.2, 0.0, 0.0);    // 4:右上	    \n"
"	EmitVertex();														\n"
"	gl_Position = position + vec4(0.0, 0.4, 0.0, 0.0);    // 5:顶部	    \n"
"   fColor = vec3(1.0, 1.0, 1.0);										\n"
"	EmitVertex();														\n"
"	EndPrimitive();														\n"
"}																	    \n"
"void main() {															\n"
"	build_house(gl_in[0].gl_Position);									\n"
"}																		\n";




void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

int main() {

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

	//Init GLEW
	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		printf("Init GLEW failed.");
		glfwTerminate();
		return -1;
	}

	glViewport(0, 0, 800, 600);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points),&points, GL_STATIC_DRAW);



	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	unsigned int geometryShader;
	geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(geometryShader, 1, &geometryShaderSource, NULL);
	glCompileShader(geometryShader);

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glAttachShader(shaderProgram, geometryShader);
	glLinkProgram(shaderProgram);

	// 位置属性
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	
	// 颜色属性
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
	
	

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);

		glDrawArrays(GL_POINTS, 0, 4);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;

}