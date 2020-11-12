#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT {
    vec3 normal;
} gs_in[];

const float MAGNITUDE = 0.4;
uniform mat4 modelMat;
layout (std140) uniform Matrices
{
	mat4 viewMat;
	mat4 projMat;
 };

void GenerateLine(mat3 normalMatrix,int index)
{
    gl_Position = projMat*viewMat*modelMat*gl_in[index].gl_Position;
	EmitVertex();

	vec4 ptOnNormal = viewMat*modelMat*gl_in[index].gl_Position + vec4( normalize(normalMatrix*gs_in[index].normal)*MAGNITUDE,0.0f);
	gl_Position = projMat*ptOnNormal;
	EmitVertex();
	EndPrimitive();
}

void main()
{
	mat3 nMat = mat3(transpose(inverse(viewMat*modelMat)));
    GenerateLine(nMat,0); // 第一个顶点法线
    GenerateLine(nMat,1); // 第二个顶点法线
    GenerateLine(nMat,2); // 第三个顶点法线
}