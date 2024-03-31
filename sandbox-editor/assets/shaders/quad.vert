#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aUV;
layout (location = 3) in int aIndex;

out vec4 oColor;

layout (std140) uniform Camera {
  mat4 viewProj;
};

#define MAX_TRANSFORM 500

layout (std140) uniform Transform {
  mat4 transforms[MAX_TRANSFORM];
};

void main() {
  gl_Position = viewProj * transforms[aIndex] * vec4(aPos, 1.f);
  oColor = aColor;
}
