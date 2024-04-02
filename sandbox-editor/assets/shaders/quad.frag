#version 330 core

out vec4 fragColor;

in vec4 inColor;
in vec3 inFragPos;
in vec2 inTexCoord;
in vec3 inNormal;

uniform bool enableLighting;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

float lightIntensity = 0.4;

struct Light
{
  vec3 pos;
  vec3 color;
};

#define MAX_NR_LIGHT 4

uniform Light lights[MAX_NR_LIGHT];

uniform int nr_lights;

vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos) {
  vec3 lightDir = normalize(light.pos - fragPos);

  vec2 toLight = light.pos.xy - fragPos.xy;
  float distance = length(toLight);

  float attenuation = 1.0 / (1.0 + 0.00005 * distance + 0.00005 * distance * distance);
  vec3 ambientColor = lightColor.rgb * 0.1f * attenuation;

  float diffuseFactor = max(dot(normal, lightDir), 0.0);
  vec3 diffuseColor = lightColor * attenuation * (1.0 - diffuseFactor);

  return ambientColor + diffuseColor;
}

void main() {
  if (!enableLighting) {
    fragColor = inColor;
    return;
  }
  /*vec2 toLight = lightPos.xy - inFragPos.xy;
  float distance = length(toLight);

  float attenuation = 1.0 / (1.0 + 0.00005 * distance + 0.00005 * distance * distance);
  vec3 ambientColor = lightColor.rgb * 0.1f * attenuation;

  vec3 lightDir = normalize(vec3(toLight, 0.0));
  vec3 normal = normalize(inNormal);
  float diffuseFactor = max(dot(normal, lightDir), 0.0);
  vec3 diffuseColor = lightColor * attenuation * (1.0 - diffuseFactor);*/

  vec3 result = vec3(0.0);

  Light light;
  light.pos = lightPos;
  light.color = lightColor;

  for (int i = 0; i < MAX_NR_LIGHT; i++)
    result += CalcPointLight(light , inNormal, inFragPos);

  fragColor = inColor * vec4(result, 1.f);
}
