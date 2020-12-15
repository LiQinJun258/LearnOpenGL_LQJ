// 计算用IBL与直接光的PBR效果
varying vec3 fragPos;
varying vec3 vNormal;

uniform vec3 albedo;
uniform float metallic;
uniform float rough;
uniform vec3 directionalLightDir;
uniform float lightStrength;
uniform samplerCube tCube;

uniform samplerCube prefilterCube0;
uniform samplerCube prefilterCube1;
uniform samplerCube prefilterCube2;
uniform samplerCube prefilterCube3;
uniform samplerCube prefilterCube4;
uniform float prefilterScale;

uniform sampler2D BRDFlut;

const float PI = 3.1415926;
vec3 F0;
// ----------------------------------------------------------------------------
//法线分布函数，采用Trowbridge-Reitz GGX进行近似
//根据迪士尼公司给出的观察以及后来被Epic Games公司采用的光照模型
//光照在法线分布函数中，采用粗糙度的平方会让光照看起来更加自然。
float distribution(vec3 halfDir, vec3 normal, float rough){
    float rough2 = rough * rough;
    float NdotH = max(dot(normal, halfDir), 0.0);
    float numerator = rough2;
    float media = NdotH * NdotH * (rough2 - 1.0) + 1.0;
    float denominator = PI * media * media;
    return numerator / max(denominator, 0.001);
}

// ----------------------------------------------------------------------------
//Schlick-GGX近似法
//用来计算观察方向的几何遮蔽(Geometry Obstruction)
//以及光线方向的几何阴影(Geometry Shadowing)
float geometrySub(float NdotL, float mappedRough){
    float numerator = NdotL;
    float denominator = NdotL * (1.0 - mappedRough) + mappedRough;
    return numerator / denominator;
}

// ----------------------------------------------------------------------------
// 采用史密斯法(Smith’s method)结合Schlick-GGX近似法，计算几何函数
float geometry(float NdotL, float NdotV, float rough){
    float mappedRough = (rough + 1.0) * (rough + 1.0) / 8.0;
    float geoLight = geometrySub(NdotL, mappedRough);
    float geoView = geometrySub(NdotV, mappedRough);
    return geoLight * geoView;
}
// ----------------------------------------------------------------------------
// 菲涅尔方程用Fresnel-Schlick近似法求近似解
vec3 fresnel(vec3 baseReflect, vec3 viewDir, vec3 halfDir){
    float HdotV = clamp(dot(halfDir, viewDir), 0.0, 1.0);
    float media = pow((1.0 - HdotV), 5.0);
    return baseReflect + (1.0 -baseReflect) * media;
}

// 直接光的计算，按照反射方程计算
vec3 explicitLighting(vec3 normal, vec3 viewDir, float NdotV){
    vec3 lightDir = normalize(directionalLightDir);
    vec3 halfDir = normalize(viewDir + lightDir);
    float NdotL = max(dot(normal, lightDir), 0.0);

    // 计算DFG
    float D = distribution(halfDir, normal, rough);
    vec3 F = fresnel(F0, viewDir, halfDir);
    float G = geometry(NdotL, NdotV, rough);

    // 为了保证能量守恒，剩下的光都会被折射
    vec3 kd = vec3(1.0) - F;

    // 因为金属不会折射光线，因此不会有漫反射。
    // 所以如果表面是金属的，我们会把系数kD 变成0
    // 如果是之间的就插值
    kd *= 1.0 - metallic;

    // 最终的结果实际上是反射方程在半球领域Ω的积分的结果
    // 但是我们实际上不需要去求积，因为是直接光照
    // 根据反射方程进行计算一根光线就可以
    vec3 difBRDF = kd * albedo / PI;
    vec3 specBRDF = D * F * G / max((4.0 * NdotV * NdotL), 0.001);
    vec3 BRDF = difBRDF + specBRDF;

    // 光强
    vec3 radiance = vec3(lightStrength);

    return BRDF * radiance * NdotL;
}

// 菲涅尔方程用Fresnel-Schlick近似法求近似解
// 粗糙度为1则没有F0
vec3 fresnelRoughness(float cosTheta, vec3 F0, float roughness){
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}

// 根据粗糙度划分的等级进行插值
vec3 samplePrefilter(vec3 cubeMapTexcoord){
    int prefilterLevel = int(floor(prefilterScale));
    float coeMix = prefilterScale - float(prefilterLevel);
    vec3 colorA, colorB, colorP;
    if(prefilterLevel == 0){
        colorA = textureCube(prefilterCube0, cubeMapTexcoord).xyz;
        colorB = textureCube(prefilterCube1, cubeMapTexcoord).xyz;
    }
    else if(prefilterLevel == 1){
        colorA = textureCube(prefilterCube1, cubeMapTexcoord).xyz;
        colorB = textureCube(prefilterCube2, cubeMapTexcoord).xyz;
    }
    else if(prefilterLevel == 2){
        colorA = textureCube(prefilterCube2, cubeMapTexcoord).xyz;
        colorB = textureCube(prefilterCube3, cubeMapTexcoord).xyz;
    }
    else if(prefilterLevel == 3){
        colorA = textureCube(prefilterCube3, cubeMapTexcoord).xyz;
        colorB = textureCube(prefilterCube4, cubeMapTexcoord).xyz;
    }
    else{
        colorA = textureCube(prefilterCube4, cubeMapTexcoord).xyz;
        colorB = textureCube(prefilterCube4, cubeMapTexcoord).xyz;
    }
    colorP = mix(colorA, colorB, coeMix);
    return colorP;
}

void main(){
    // 法向量与观察向量
    vec3 normal = normalize(vNormal);
    vec3 viewDir = normalize(cameraPosition - fragPos);

    // 反射向量
    vec3 R = reflect(-viewDir, normal);

    // cos值
    float NdotV = max(dot(normal, viewDir), 0.0);

    // 在PBR金属流中，我们简单地认为大多数的绝缘体在F0为0.04的时候看起来视觉上是正确的
    // 根据表面的金属性来改变F0值， 并且在原来的F0和反射率中插值计算F0
    F0 = mix(vec3(0.04), albedo, metallic);

    // 计算直接光
    vec3 colorFromLight = explicitLighting(normal, viewDir, NdotV);

    // 计算环境光的菲涅尔方程系数kd
    vec3 F = fresnelRoughness(NdotV, F0, rough);
    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;

    // 采样环境光预积分 的 环境图（镜面反射的第一部分）
    vec3 prefilteredColor = samplePrefilter(R);
    // 采样环境光预积分 的 BRDF
    vec2 brdf = texture2D(BRDFlut, vec2(NdotV, rough)).xy;
    // 采样并计算环境光预积分 的 漫反射部分
    vec3 ambientDiffuse = kD * albedo * textureCube(tCube, normal).rgb;
    // 计算镜面反射部分
    vec3 ambientSpecular = prefilteredColor * (F * brdf.x + brdf.y);
    vec3 ambient = ambientDiffuse + ambientSpecular;
    // 加上直接光的效果
    vec3 color = ambient + colorFromLight;

    // 我们希望所有光照的输入都尽可能的接近他们在物理上的取值
    // 这样他们的反射率或者说颜色值就会在色谱上有比较大的变化空间。
    // 所以进行色调映射，Reinhard色调映射算法平均地将所有亮度值分散到LDR上
    color = color / (color + vec3(1.0));
    // 直到现在，我们假设的所有计算都在线性的颜色空间中进行的，因此我们需要在着色器最后做伽马矫正
    color = pow(color, vec3(1.0/2.2));

    gl_FragColor = vec4(color, 1.0);

}