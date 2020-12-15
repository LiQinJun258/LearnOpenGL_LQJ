// 根据方程，预计算漫反射积分
uniform samplerCube tCube;

varying vec3 cubeMapTexcoord;

const float PI = 3.1415926;
// 黎曼积分步长
const float sampleDelta = 0.025;//0.025;

void main(){
    // 在切线空间的半球采样
    // 世界向量充当原点的切线曲面的法线，立方体归一化以后就是球的方向向量，也就是法线
    // 给定此法线，计算环境的所有传入辐射。
    vec3 N = normalize(cubeMapTexcoord);
    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 right = normalize(cross(up, N));
    up = normalize(cross(N, right));

    float nrSamples = 0.0;
    vec3 irradiance = vec3(0.0);
    // 两重循环，获取采样方向向量
    for(float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta){
        float cosPhi = cos(phi);
        float sinPhi = sin(phi);

        for(float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta){
            float cosTheta = cos(theta);
            float sinTheta = sin(theta);

            // 在切线空间中，从球面坐标到笛卡尔坐标
            vec3 tangentDir = vec3(cosPhi * sinTheta, sinPhi * sinTheta, cosTheta);
            // 切线空间到世界空间
            // 没有往常一样构建TBN变换矩阵，直接用计算了的切线空间的三个基向量。
            vec3 worldDir = tangentDir.x * right + tangentDir.y * up + tangentDir.z * N;
            irradiance += textureCube(tCube, worldDir).rgb * cosTheta * sinTheta;
            nrSamples++;
        }
    }
    irradiance = irradiance * (1.0 / float(nrSamples)) * PI;

    gl_FragColor = vec4(irradiance, 1.0);
}