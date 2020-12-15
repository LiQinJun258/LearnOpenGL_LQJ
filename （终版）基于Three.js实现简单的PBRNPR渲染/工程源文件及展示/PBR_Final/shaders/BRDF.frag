// 预计算 BRDF
// 分成两部分
varying vec2 texcoord;

const float PI = 3.1415926;

// Van Der Corput 序列生成（用于不能移位操作的WebGL）
float VanDerCorpus(int n, int base){
    float invBase = 1.0 / float(base);
    float denom   = 1.0;
    float result  = 0.0;

    for(int i = 0; i < 32; ++i){
        if(n > 0){
            denom   = mod(float(n), 2.0);
            result += denom * invBase;
            invBase = invBase / 2.0;
            n       = int(float(n) / 2.0);
        }
    }

    return result;
}

// 超均匀分布序列（Low-discrepancy Sequence）——Hammersley序列
// 利用Van Der Corput 序列生成Hammersley序列
vec2 HammersleyNoBitOps(int i, int N){
    return vec2(float(i)/float(N), VanDerCorpus(i, 2));
}

// 利用法线分布函数与余弦映射结合起来做重要性采样
vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness){
    float a = roughness * roughness;

    // 在球面空间将法线分布函数与余弦映射结合起来做重要性采样
    // 将Hammersley序列x_i=(u,v)^T 映射到(\phi,\theta)，然后转换成笛卡尔坐标下的向量形式。
    float phi = 2.0 * PI * Xi.x;
    float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0) * Xi.y));
    float sinTheta = sqrt(1.0 - cosTheta * cosTheta);

    // 从球面到笛卡尔坐标
    vec3 H;
    H.x = cos(phi) * sinTheta;
    H.y = sin(phi) * sinTheta;
    H.z = cosTheta;

    // 从切线空间变换到世界空间
     vec3 up        = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
     vec3 tangent   = normalize(cross(up, N));
     vec3 bitangent = cross(N, tangent);

     vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
     return normalize(sampleVec);
}

//Schlick-GGX近似法
//用来计算观察方向的几何遮蔽(Geometry Obstruction)
//以及光线方向的几何阴影(Geometry Shadowing)
float geometrySub(float NdotL, float mappedRough){
    float numerator = NdotL;
    float denominator = NdotL * (1.0 - mappedRough) + mappedRough;
    return numerator / denominator;
}

// 采用史密斯法(Smith’s method)结合Schlick-GGX近似法，计算几何函数
float geometry(float NdotL, float NdotV, float rough){
    float mappedRough = rough * rough / 2.0; //   IBL的特有系数
    float geoLight = geometrySub(NdotL, mappedRough);
    float geoView = geometrySub(NdotV, mappedRough);
    return geoLight * geoView;
}

// 进行BRDF的预积分
void main(){
    // 构建这样的一个二维查找表，它的横轴坐标取值为NdotV，纵轴坐标取值为粗糙度roughness。
    float NdotV = texcoord.x;
    float roughness = texcoord.y;

    // 认为N在切线空间，求出V
    vec3 N = vec3(0.0, 0.0, 1.0);
    vec3 V;
    V.z = NdotV;
    V.x = sqrt(1.0 - NdotV * NdotV);
    V.y = 0.0;

    const int SAMPLE_COUNT = 1024;
    float A = 0.0;
    float B = 0.0;
    for(int i = 0; i < SAMPLE_COUNT; i++){
        // 生成Hammersley序列进行采样
        vec2 Xi = HammersleyNoBitOps(i, SAMPLE_COUNT);

        // 重要性采样偏移采样向量
        // 获得了一个采样向量，该向量大体围绕着预估的微表面的半程向量
        vec3 H = ImportanceSampleGGX(Xi, N, roughness);

        // 反推了光照方向
        vec3 L = normalize(2.0 * dot(V, H) * H - V);

        float NdotL = max(dot(N, L), 0.0);
        float NdotH = max(H.z, 0.0);
        float VdotH = max(dot(V, H), 0.0);

        // 如果该光照方向和法线点积大于0，则可以采用纳入预积分计算
        if(NdotL > 0.0){
            // 计算两部分的积分
            // 计算系数
            float G = geometry(NdotL, NdotV, roughness);
            float G_Vis = (G / (NdotV * NdotH)) * VdotH;
            // 计算乘方
            float Fc = pow(1.0 - VdotH, 5.0);

            // 计算两部分积分 把A，B存成色值，方便后面直接读取乘以F0
            A += (1.0 - Fc) * G_Vis;
            B += Fc * G_Vis;
        }
    }
    A /= float(SAMPLE_COUNT);
    B /= float(SAMPLE_COUNT);

    gl_FragColor = vec4(A, B, 0.0, 1.0);
}