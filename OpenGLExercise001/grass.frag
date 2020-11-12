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
void main(){	
	vec4 texColor = texture(material.diffuse, TexCoord);
	if(texColor.a < 0.5)
        discard;
	FragColor = texColor;
}
