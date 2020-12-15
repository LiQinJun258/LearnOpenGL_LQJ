// 预计算镜面反射积分 的 环境贴图部分
// 构建二维的Hammersley序列，实现将法线分布函数与余弦映射结合起来做重要性采样，计算蒙特卡洛积分
// 将粗糙度换分成5个等级，每个等级根据当前的粗糙度进行重要性采样
uniform samplerCube tCube;
uniform float roughness;

varying vec3 cubeMapTexcoord;

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

void main(){
    vec3 N = normalize(cubeMapTexcoord);
    vec3 V = N;
    vec3 R = N;//假设视角方向是镜面反射方向,总是等于输出采样方向，总是等于法线
               //假设近似会导致NDF波瓣从各向异性变为各向同性
               //最明显的现象就是在掠射角（grazing angles）看表面时没法得到拖长的反射

    const int SAMPLE_COUNT = 1024;
    vec3 color = vec3(0.0);
    float totalWeight = 0.0;
    // 进行采样
    for(int i = 0; i < SAMPLE_COUNT; i++){
        // 生成Hammersley序列进行采样
        vec2 Xi = HammersleyNoBitOps(i, SAMPLE_COUNT);

        // 重要性采样偏移采样向量
        // 获得了一个采样向量，该向量大体围绕着预估的微表面的半程向量
        vec3 H = ImportanceSampleGGX(Xi, N, roughness);
        // 反推了光照方向
        vec3 L = normalize(2.0 * dot(V, H) * H - V);
        // 如果该光照方向和法线点积大于0，则可以采用纳入预积分计算
        float NdotL = max(dot(N, L), 0.0);
        if(NdotL > 0.0){
            // Ephic Games公司发现再乘上一个权重cos效果更加，因而在采样之后，又乘以了NdotL
            color += textureCube(tCube, L).rgb * NdotL;
            totalWeight += NdotL;
        }
    }
    color /= totalWeight;

    gl_FragColor = vec4(color, 1.0);
}