#version 330 core			

//着色点和法向
in vec3 FragPos;
in vec3 Normal;

in vec2 TexCoord;
struct Material {
    vec3 ambient;
    sampler2D diffuse;
    sampler2D specular;
	sampler2D emission;
    float shininess;
}; 
uniform Material material;

struct LightDirectional{
    vec3 dirToLight;
    vec3 color;
};
uniform LightDirectional lightD1;


out vec4 FragColor;			

uniform vec3 objColor;

uniform vec3 ambientColor;
uniform vec3 cameraPos;

uniform float time;



vec3 CalcLightDirectional(LightDirectional light, vec3 uNormal, vec3 dirToCamera){

	//diffuse
	float diffIntensity = max(dot(uNormal, light.dirToLight), 0.0);
	vec3 diffuseColor =   diffIntensity * texture(material.diffuse,TexCoord).rgb * light.color;
	//Blinn-Phong specular
	vec3 halfwarDir = normalize(light.dirToLight + dirToCamera);
	float specularAmount = pow(max(dot(uNormal, halfwarDir), 0.0),material.shininess);
	vec3 specularColor = texture(material.specular,TexCoord).rgb  * specularAmount * light.color;
	
	vec3 result = diffuseColor+specularColor;
	return result;
}

void main(){
	vec3 finalResult = vec3(0,0,0);
	vec3 uNormal = normalize(Normal);
	vec3 dirToCamera = normalize(cameraPos - FragPos);

	//ambient
	vec3 ambient=  ambientColor * texture(material.specular,TexCoord).rgb;
	
	finalResult += CalcLightDirectional(lightD1, uNormal, dirToCamera);

	
	finalResult += ambient;


	

	FragColor=vec4(finalResult,1.0);
	//FragColor=vec4(1.0,1.0,1.0,1.0);
}
