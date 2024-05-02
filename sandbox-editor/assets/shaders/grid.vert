#version 330 core

layout (location = 0) in vec3 aPos;

layout(location = 1) out vec3 nearPoint;
layout(location = 2) out vec3 farPoint;
layout(location = 3) out mat4 fragViewProj;


uniform mat4 viewProj;

vec3 UnprojectPoint(float x, float y, float z, mat4 viewProj)
{
	mat4 matInv = inverse(viewProj);

	vec4 point = matInv * vec4(x, y, z, 1.0);

	return point.xyz / point.w;
}

void main()
{
	vec3 p = aPos;
	fragViewProj = viewProj;
	nearPoint = UnprojectPoint(p.x, p.y, 0.0, viewProj);
	farPoint = UnprojectPoint(p.x, p.y, 1.0, viewProj);
	gl_Position = vec4(aPos, 1.0);
}
