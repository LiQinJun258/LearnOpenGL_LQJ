#version 330 core									  
layout(location = 0) in vec3 aPos;   // λ�ñ���������λ��ֵΪ 0
layout(location = 1) in vec3 aNormal; // ������������λ��ֵΪ 1
layout(location = 2) in vec2 aTexCoord; // uv����������λ��ֵΪ 2


//out vec4 vertexColor;
out vec2 TexCoord;


//��ɫ��ͷ���
out vec3 FragPos;
out vec3 Normal;


//uniform mat4 transform;
uniform mat4 modelMat;

layout (std140) uniform Matrices
{
	mat4 viewMat;
	mat4 projMat;
 };


void main(){										   
   gl_Position =  projMat * viewMat * modelMat * vec4(aPos.xyz,1.0);   
   Normal =mat3(transpose(inverse(modelMat)))*aNormal;
   FragPos=(modelMat * vec4(aPos.xyz,1.0)).xyz;
   //vertexColor = vec4(aColor,1.0);	
   //TexCoord = aTexCoord;
   TexCoord=aTexCoord;

}