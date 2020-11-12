#version 330 core	

 
in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;


uniform sampler2D normalMap; 
uniform sampler2D DiffuseTexture; 
uniform sampler2D depthMap;

out vec4 FragColor;			

uniform vec3 lightColor;
uniform vec3 ambientColor;
uniform float height_scale;

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{ 

	// number of depth layers
	const float minLayers = 8.0;
	const float maxLayers = 32.0;
	float numLayers = mix(maxLayers, minLayers, max(dot(vec3(0.0, 0.0, 1.0), viewDir), 0.0));  
    // calculate the size of each layer
    float layerDepth = 1.0 / numLayers;
    // depth of current layer
    float currentLayerDepth = 0.0;
    // the amount to shift the texture coordinates per layer (from vector P)
    vec2 P = viewDir.xy * height_scale; 
    vec2 deltaTexCoords = P / numLayers;


	// get initial values
	vec2  currentTexCoords  = texCoords;
	float currentDepthMapValue = texture(depthMap, currentTexCoords).r;
  
	while(currentLayerDepth < currentDepthMapValue)
	{
		// shift texture coordinates along direction of P
		currentTexCoords -= deltaTexCoords;
		// get depthmap value at current texture coordinates
		currentDepthMapValue = texture(depthMap, currentTexCoords).r;  
		// get depth of next layer
		currentLayerDepth += layerDepth;  
	}

	// get texture coordinates before collision (reverse operations)
	vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

	// get depth after and before collision for linear interpolation
	float afterDepth  = currentDepthMapValue - currentLayerDepth;
	float beforeDepth = texture(depthMap, prevTexCoords).r - currentLayerDepth + layerDepth;
 
	// interpolation of texture coordinates
	float weight = afterDepth / (afterDepth - beforeDepth);
	vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

	return finalTexCoords; 
    
}


void main(){
	vec3 finalResult = vec3(0,0,0);
	// 从法线贴图范围[0,1]获取法线
    // 将法线向量转换为范围[-1,1]

	// offset texture coordinates with Parallax Mapping
    vec3 dirToCamera = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec2 texCoords = ParallaxMapping(fs_in.TexCoords,  dirToCamera);
	if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)
		discard;
	//texCoords=fs_in.TexCoords;

    // then sample textures with new texture coords

	vec3 uNormal =texture(normalMap, texCoords).rgb;

    uNormal =normalize( uNormal * 2.0f - 1.0f);   


	
	//ambient
	vec3 ambient=  ambientColor * texture(DiffuseTexture,texCoords).rgb;

	
	vec3 dirToLight=normalize(fs_in.TangentLightPos-fs_in.TangentFragPos);	
	//diffuse
	float diffIntensity = max(dot(dirToLight,uNormal), 0.0);
	vec3 diffuseColor =   diffIntensity * texture(DiffuseTexture,texCoords).rgb * lightColor;
	//Blinn-Phong specular
	vec3 halfwarDir = normalize(dirToLight + dirToCamera);
	float specularAmount = pow(max(dot(uNormal, halfwarDir), 0.0),32.0f);
	vec3 specularColor = vec3(0.2) * specularAmount * lightColor;
	vec3 result = (diffuseColor+specularColor);

	finalResult += result;
	finalResult += ambient;
	
	FragColor=vec4(finalResult,1.0);


}
