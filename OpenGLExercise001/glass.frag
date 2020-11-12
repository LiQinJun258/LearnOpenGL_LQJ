#version 330 core									
in vec4 vertexColor;
in vec2 TexCoord;
struct Material {
    vec3 ambient;
    sampler2D diffuse;
    sampler2D specular;
	sampler2D emission;
    float shininess;
}; 
uniform Material material;
out vec4 FragColor;								   
void main()
{             
    FragColor = texture(material.diffuse, TexCoord);
}