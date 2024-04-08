#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 uViewProj;

void main()
{
    gl_Position = uViewProj * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    TexCoord = aTex;
}
