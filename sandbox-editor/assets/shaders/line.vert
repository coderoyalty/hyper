#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in int aEntityId;

out vec4 oColor;
flat out int oEntityId;

layout (std140) uniform Camera {
  mat4 viewProj;
};

void main() {
  gl_Position = viewProj * vec4(aPos, 1.f);
  oColor = aColor;
  oEntityId = aEntityId;
}
