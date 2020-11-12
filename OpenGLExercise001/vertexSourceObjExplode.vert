#version 330 core
layout(location = 0) in vec3 aPos;   // 位置变量的属性位置值为 0
layout(location = 1) in vec3 aNormal; // 法向量的属性位置值为 1
layout(location = 2) in vec2 aTexCoord; // uv变量的属性位置值为 2

out VS_OUT {
    vec2 texCoords;
	
} vs_out;



//着色点和法向
//out vec3 FragPos;
out mat4 modelMat1;


uniform mat4 modelMat;
layout (std140) uniform Matrices
{
	mat4 viewMat;
	mat4 projMat;
 };



void main()
{
	modelMat1 = modelMat;
    vs_out.texCoords = aTexCoord;
	gl_Position =   vec4(aPos.xyz,1.0); 
	
//	FragPos=(modelMat * vec4(aPos.xyz,1.0)).xyz;
	
	
}