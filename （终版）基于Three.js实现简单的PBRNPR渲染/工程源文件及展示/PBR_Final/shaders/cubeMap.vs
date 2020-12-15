varying vec3 cubeMapTexcoord;
//正常渲染立方体
void main(){
    cubeMapTexcoord = position;
    gl_Position = projectionMatrix * modelViewMatrix * vec4( position, 1.0 );
}