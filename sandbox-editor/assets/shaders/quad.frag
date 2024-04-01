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

void main() {
  if (!enableLighting) {
    fragColor = inColor;
    return;
  }
  vec2 toLight = lightPos.xy - inFragPos.xy;
  float distance = length(toLight);

  float attenuation = 1.0 / (1.0 + 0.00005 * distance + 0.00005 * distance * distance);
  vec3 ambientColor = lightColor.rgb * 0.1f;

  vec3 lightDir = normalize(vec3(toLight, -1.0));
  vec3 normal = normalize(inNormal);
  float diffuseFactor = max(dot(normal, lightDir), 0.0);
  vec3 diffuseColor = lightColor * attenuation * (1.0 - diffuseFactor);

  fragColor = inColor * vec4(ambientColor + diffuseColor, 1.f);
}
