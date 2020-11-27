varying vec3 fragPos;
varying vec3 vNormal;
//获得着色点，法向量，输出顶点位置
void main(){
    fragPos = (modelMatrix * vec4(position, 1.0)).xyz;
    vNormal = mat3(modelMatrix) * normal;
    gl_Position = projectionMatrix * modelViewMatrix * vec4( position, 1.0 );
}