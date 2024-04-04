#version 330 core

out vec4 fragColor;

in vec4 inColor;
in vec3 inFragPos;
in vec2 inTexCoord;
in vec3 inNormal;

uniform bool enableLighting;

struct Light
{
  vec3 pos;
  vec3 color;
};

#define MAX_NR_LIGHT 32

layout (std140) uniform Lights {
  Light lights[MAX_NR_LIGHT];
};

uniform int noLights;

vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos) {
  vec3 lightDir = normalize(light.pos - fragPos);

  vec2 toLight = light.pos.xy - fragPos.xy;
  float distance = length(toLight);

  float attenuation = 1.0 / (1.0 + 0.001 * distance + 0.001 * distance * distance);
  vec3 ambientColor = light.color * 0.1f * attenuation;

  float diffuseFactor = max(dot(normal, lightDir), 0.0);
  vec3 diffuseColor = light.color * attenuation * (1.0 - diffuseFactor);

  return ambientColor + diffuseColor;
}

void main() {
  if (!enableLighting) {
    fragColor = inColor;
    return;
  }

  vec3 result = vec3(0.0);

  for (int i = 0; i < noLights; i++)
    result += CalcPointLight(lights[i] , inNormal, inFragPos);

  fragColor = inColor * vec4(result, 1.f);
}
