varying vec3 fragPos;
varying vec3 vNormal;

uniform vec3 albedo;
uniform float metallic;
uniform float roughness;
uniform float ao;

uniform vec3 pointLightsPosition[4];
uniform vec3 pointLightsColors[4];

const float PI = 3.1415926;

// ----------------------------------------------------------------------------
//法线分布函数，采用Trowbridge-Reitz GGX进行近似
//根据迪士尼公司给出的观察以及后来被Epic Games公司采用的光照模型
//光照在法线分布函数中，采用粗糙度的平方会让光照看起来更加自然。
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / max(denom, 0.001); // 为了避免当roughness=0.0 and NdotH=1.0时出现除零错误。
}
// ----------------------------------------------------------------------------
//Schlick-GGX近似法
//用来计算观察方向的几何遮蔽(Geometry Obstruction)
//以及光线方向的几何阴影(Geometry Shadowing)
float GeometrySchlickGGX(float NdotV, float roughness)
{
    //k针对直接光照进行了粗糙度的重映射(Remapping)
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
// 采用史密斯法(Smith’s method)结合Schlick-GGX近似法，计算几何函数
float GeometrySmith(float NdotV, float NdotL, float roughness)
{

    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
// 菲涅尔方程用Fresnel-Schlick近似法求近似解
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}
// ----------------------------------------------------------------------------

void main()
{
    // 获得法向量，观察向量
    vec3 N = normalize(vNormal);
    vec3 V = normalize(cameraPosition - fragPos);

    // 获取N与V的点乘
    float NdotV = max(dot(N, V), 0.0);
    // 在PBR金属流中，我们简单地认为大多数的绝缘体在F0为0.04的时候看起来视觉上是正确的
    vec3 F0 = vec3(0.04);
    // 根据表面的金属性来改变F0这个值， 并且在原来的F0和反射率中插值计算F0
    F0 = mix(F0, albedo, metallic);

    // 反射方程的计算
    vec3 Lo = vec3(0.0);
    for(int i = 0; i < 4; ++i)
    {
        // 计算每一根光线的radiance（因为是直接光照，取代了积分）
        // 计算光源向量，半程向量
        vec3 L = normalize(pointLightsPosition[i] - fragPos);
        vec3 H = normalize(V + L);
        // 获取N与L的点乘
        float NdotL = max(dot(N, L), 0.0);

        // 计算衰减，也就是计算从点光源发出的Radiance变成物体接收到的Irradiance
        float distance = length(pointLightsPosition[i] - fragPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = pointLightsColors[i] * attenuation;

        // 计算Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);
        float G   = GeometrySmith(NdotV , NdotL, roughness);
        vec3 F    = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);

        // 计算反射分量
        vec3 nominator    = NDF * G * F;
        // 防止因为 NdotV=0.0 或者 NdotL=0.0 让除数变成 0
        vec3 specular = nominator / max((4.0 * NdotV * NdotL), 0.001);

        // 菲涅尔方程直接给出了kS
        vec3 kS = F;
        // 为了保证能量守恒，剩下的光都会被折射
        vec3 kD = vec3(1.0) - kS;
        // 因为金属不会折射光线，因此不会有漫反射。
        // 所以如果表面是金属的，我们会把系数kD 变成0
        // 如果是之间的就插值
        kD *= 1.0 - metallic;

        // 最终的结果Lo，实际上是反射方程在半球领域Ω的积分的结果
        // 但是我们实际上不需要去求积，因为是直接光照
        // 所以对于所有可能的入射光线方向我们知道只有4个方向的入射光线会影响片段(像素)的着色
        // 根据反射方程进行计算
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;
    }

    // 剩下的工作就是加一个环境光照项给结果，然后我们就拥有了片段的最后颜色
    vec3 ambient = vec3(0.03) * albedo * ao;

    vec3 color = ambient + Lo;

    // 我们希望所有光照的输入都尽可能的接近他们在物理上的取值，这样他们的反射率或者说颜色值就会在色谱上有比较大的变化空间。
    // 所以进行色调映射，Reinhard色调映射算法平均地将所有亮度值分散到LDR上
    color = color / (color + vec3(1.0));
    // 直到现在，我们假设的所有计算都在线性的颜色空间中进行的，因此我们需要在着色器最后做伽马矫正
    color = pow(color, vec3(1.0/2.2));

    gl_FragColor = vec4(color, 1.0);
}