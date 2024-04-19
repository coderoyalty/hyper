#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aUV;


layout (std140) uniform Camera {
  mat4 viewProj;
};

out vec4 inColor;
out vec2 inTexCoord;

void main() {
	gl_Position = viewProj * vec4(aPos, 1.0);
	inTexCoord = aUV;
	inColor = aColor;
}
