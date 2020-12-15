uniform samplerCube tCube;

varying vec3 cubeMapTexcoord;
// hdr文件存储每个浮点值的方式
// 每个通道存储 8 位，再以 alpha 通道存放指数
// 因此利用这种方式解码
vec3 hdrDecode(vec4 encoded){
    float exponent = encoded.a * 256.0 - 128.0;
    vec3 mantissa = encoded.rgb;
    return exp2(exponent) * mantissa;
}

void main(){
    vec4 color = textureCube(tCube, cubeMapTexcoord);
    vec3 envColor = hdrDecode(color);
    gl_FragColor = vec4(envColor, 1.0);
}