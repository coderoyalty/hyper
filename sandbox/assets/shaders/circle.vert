#version 330 core

layout (location = 0) in vec3 aWorldPosition;
layout (location = 1) in vec3 aLocalPosition;
layout (location = 2) in vec4 aColor;
layout (location = 3) in float aThickness;
layout (location = 4) in float aFade;

layout (std140) uniform Camera {
	mat4 uViewProj;	
};

out vec3 localPosition;
out vec4 color;
out float thickness;
out float fade;

void main() {
	localPosition = aLocalPosition;
	color = aColor;
	thickness = aThickness;
	fade = aFade;

	gl_Position = uViewProj * vec4(aWorldPosition, 1.f);
}
