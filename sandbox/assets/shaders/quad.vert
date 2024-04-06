#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aUV;
layout (location = 3) in int aTransformIndex;
layout (location = 4) in float aTextureIndex;

layout (std140) uniform Camera {
  mat4 viewProj;
};

#define MAX_TRANSFORM 1000

layout (std140) uniform Transform {
  mat4 transforms[MAX_TRANSFORM];
};

uniform bool enableLighting;

out vec4 inColor;
out vec3 inFragPos;
out vec2 inTexCoord;
out vec3 inNormal;
flat out float textureIndex;

void main() {
  mat4 model = transforms[aTransformIndex];

  inColor = aColor;
  textureIndex = aTextureIndex;

  if (enableLighting) {
    inFragPos = vec3(model * vec4(aPos, 1.0));
    inNormal = mat3(transpose(inverse(model))) * vec3(0.0, 0.0, 1.0);
    
    inTexCoord = aUV;

    gl_Position = viewProj * vec4(inFragPos, 1.0);

    return;
  }
  
  gl_Position = viewProj * model * vec4(aPos, 1.0);
}
