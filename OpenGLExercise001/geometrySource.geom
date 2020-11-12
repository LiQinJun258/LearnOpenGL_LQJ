#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec2 texCoords;
} gs_in[];


out vec2 TexCoord; 
out vec3 FragPos;
uniform float time1;



out vec3 Normal;

uniform mat4 modelMat;

vec4 explodePosition;
layout (std140) uniform Matrices
{
	mat4 viewMat;
	mat4 projMat;
 };



vec4 explode(vec4 position, vec3 normal)
{
    float magnitude = 2.0;
    vec3 direction = normal * (( sin(time1/1.5) + 1.0) / 2.0) * magnitude; 
    return position + vec4(direction.xy,0.0, 0.0);
}

vec3 GetNormal()
{
   vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
   vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
   return normalize(cross(b,a));
}

void main() {    
    //vec3 normal = normalize(mat3(transpose(inverse(modelMat1)))*GetNormal());
	vec3 normal = GetNormal();
	Normal = normal;
	
	explodePosition=modelMat *explode(gl_in[0].gl_Position, normal);
	FragPos=explodePosition.xyz;
    gl_Position = projMat * viewMat * explodePosition;
	
    TexCoord = gs_in[0].texCoords;
    EmitVertex();

	explodePosition=modelMat *explode(gl_in[1].gl_Position, normal);
	FragPos=explodePosition.xyz;
    gl_Position = projMat * viewMat *explodePosition;
	
    TexCoord = gs_in[1].texCoords;
    EmitVertex();

	explodePosition=modelMat *explode(gl_in[2].gl_Position, normal);
	FragPos=explodePosition.xyz;
    gl_Position = projMat * viewMat *explodePosition;
    TexCoord = gs_in[2].texCoords;
    EmitVertex();

    EndPrimitive();
}