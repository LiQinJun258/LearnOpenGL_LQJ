#version 330 core			

//着色点和法向
in vec3 FragPos;
in vec3 Normal;
uniform mat4 modelMat;
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
uniform LightDirectional lightD;

struct LightPoint{
    vec3 pos;
    vec3 color;
	float constant;
	float linear;
	float quadratic;
};
uniform LightPoint lightP0;
uniform LightPoint lightP1;
uniform LightPoint lightP2;
uniform LightPoint lightP3;

struct LightSpot{
    vec3 pos;
    vec3 color;
	float constant;
	float linear;
	float quadratic;
	vec3 dirToLight;
	float cosPhyInner;
	float cosPhyOuter;
};
uniform LightSpot lightS;




out vec4 FragColor;			

uniform vec3 objColor;

uniform vec3 ambientColor;
uniform vec3 cameraPos;

uniform float time;

uniform float What;

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

vec3 CalcLightPoint(LightPoint light, vec3 uNormal, vec3 dirToCamera){
	//attenuation
	vec3 dirToLight=light.pos-FragPos;
	float dist =length(dirToLight);
	float attenuation= 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));
	//diffuse
	float diffIntensity = max(dot(uNormal, normalize(dirToLight)), 0.0);
	vec3 diffuseColor =   diffIntensity * texture(material.diffuse,TexCoord).rgb * light.color;
	//Blinn-Phong specular
	vec3 halfwarDir = normalize(dirToLight + dirToCamera);
	float specularAmount = pow(max(dot(uNormal, halfwarDir), 0.0),material.shininess);
	vec3 specularColor = texture(material.specular,TexCoord).rgb  * specularAmount * light.color;
	
	vec3 result = (diffuseColor+specularColor)*attenuation;
	return result;
}

vec3 CalcLightSpot(LightSpot light, vec3 uNormal, vec3 dirToCamera){
	//attenuation
	vec3 dirToLight=light.pos-FragPos;
	float dist =length(dirToLight);
	float attenuation= 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));
	//diffuse
	float diffIntensity = max(dot(uNormal, normalize(dirToLight)), 0.0);
	vec3 diffuseColor =   diffIntensity * texture(material.diffuse,TexCoord).rgb * light.color;
	//Blinn-Phong specular
	vec3 halfwarDir = normalize(dirToLight + dirToCamera);
	float specularAmount = pow(max(dot(uNormal, halfwarDir), 0.0),material.shininess);
	vec3 specularColor = texture(material.specular,TexCoord).rgb  * specularAmount * light.color;
	
	//spot
	float spotRatio;
	
	float cosTheta = dot( normalize(-1*dirToLight) , -1*light.dirToLight );
	
	if(cosTheta  > light.cosPhyInner){
		//inside
		spotRatio=1;
	}else if(cosTheta > light.cosPhyOuter){
		//middle
		spotRatio=(cosTheta-light.cosPhyOuter)/(light.cosPhyInner-light.cosPhyOuter);
	}else{
		//outside
		spotRatio=0;
	}
	vec3 result=(diffuseColor+specularColor)*attenuation*spotRatio;
	return result;
}
void main(){
	vec3 finalResult = vec3(0,0,0);
	vec3 uNormal = normalize(Normal);
	vec3 dirToCamera = normalize(cameraPos - FragPos);

	//emission   32 distance
	float eDistance  = length(cameraPos-FragPos);
	float eCoefficient= 1.0 / (1.0f + 0.14f * eDistance + 0.07f * (eDistance * eDistance));
	vec3 emission;
	if( texture(material.specular,TexCoord).rgb== vec3(0,0,0) ){
		emission=   texture(material.emission,TexCoord).rgb;
		//fun
		emission = texture(material.emission,TexCoord + vec2(0.0,time/2)).rgb;//moving
		emission =  emission * (sin(time) * 0.5 + 0.5) * 2.0 * eCoefficient;//fading
	}
	
	//ambient
	vec3 ambient= material.ambient * ambientColor * texture(material.diffuse,TexCoord).rgb;
	
	finalResult += CalcLightDirectional(lightD, uNormal, dirToCamera);
//	finalResult += CalcLightPoint(lightP0, uNormal, dirToCamera);
//	finalResult += CalcLightPoint(lightP1, uNormal, dirToCamera);
//	finalResult += CalcLightPoint(lightP2, uNormal, dirToCamera);
//	finalResult += CalcLightPoint(lightP3, uNormal, dirToCamera);
//	finalResult += CalcLightSpot(lightS, uNormal, dirToCamera);

	
	if(What==1){
		finalResult += ambient;
	}else{
		finalResult += emission+ambient;
	}
	FragColor=vec4(finalResult,1.0);
	//FragColor=vec4(1.0,1.0,1.0,1.0);
}
