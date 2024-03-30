#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

out vec4 oColor;

layout (std140) uniform Camera {
  mat4 viewProj;
};

void main() {
  gl_Position = viewProj * vec4(aPos, 1.f);
  oColor = aColor;
}
