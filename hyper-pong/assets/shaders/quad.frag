#version 330 core

out vec4 fragColor;

in vec4 inColor;
in vec3 inFragPos;
in vec2 inTexCoord;
in vec3 inNormal;
flat in float textureIndex;
flat in float inTilingFactor;


uniform sampler2D textures[32];
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
  vec3 diffuseColor = light.color * attenuation * (1.f - diffuseFactor);

  return ambientColor + diffuseColor;
}

void main() {

  vec4 texColor = inColor;
  switch(int(textureIndex))
  {
    case  0: texColor *= texture(textures[ 0], inTexCoord * inTilingFactor); break;
    case  1: texColor *= texture(textures[ 1], inTexCoord * inTilingFactor); break;
    case  2: texColor *= texture(textures[ 2], inTexCoord * inTilingFactor); break;
    case  3: texColor *= texture(textures[ 3], inTexCoord * inTilingFactor); break;
    case  4: texColor *= texture(textures[ 4], inTexCoord * inTilingFactor); break;
    case  5: texColor *= texture(textures[ 5], inTexCoord * inTilingFactor); break;
    case  6: texColor *= texture(textures[ 6], inTexCoord * inTilingFactor); break;
    case  7: texColor *= texture(textures[ 7], inTexCoord * inTilingFactor); break;
    case  8: texColor *= texture(textures[ 8], inTexCoord * inTilingFactor); break;
    case  9: texColor *= texture(textures[ 9], inTexCoord * inTilingFactor); break;
    case 10: texColor *= texture(textures[10], inTexCoord * inTilingFactor); break;
    case 11: texColor *= texture(textures[11], inTexCoord * inTilingFactor); break;
    case 12: texColor *= texture(textures[12], inTexCoord * inTilingFactor); break;
    case 13: texColor *= texture(textures[13], inTexCoord * inTilingFactor); break;
    case 14: texColor *= texture(textures[14], inTexCoord * inTilingFactor); break;
    case 15: texColor *= texture(textures[15], inTexCoord * inTilingFactor); break;
    case 16: texColor *= texture(textures[16], inTexCoord * inTilingFactor); break;
    case 17: texColor *= texture(textures[17], inTexCoord * inTilingFactor); break;
    case 18: texColor *= texture(textures[18], inTexCoord * inTilingFactor); break;
    case 19: texColor *= texture(textures[19], inTexCoord * inTilingFactor); break;
    case 20: texColor *= texture(textures[20], inTexCoord * inTilingFactor); break;
    case 21: texColor *= texture(textures[21], inTexCoord * inTilingFactor); break;
    case 22: texColor *= texture(textures[22], inTexCoord * inTilingFactor); break;
    case 23: texColor *= texture(textures[23], inTexCoord * inTilingFactor); break;
    case 24: texColor *= texture(textures[24], inTexCoord * inTilingFactor); break;
    case 25: texColor *= texture(textures[25], inTexCoord * inTilingFactor); break;
    case 26: texColor *= texture(textures[26], inTexCoord * inTilingFactor); break;
    case 27: texColor *= texture(textures[27], inTexCoord * inTilingFactor); break;
    case 28: texColor *= texture(textures[28], inTexCoord * inTilingFactor); break;
    case 29: texColor *= texture(textures[29], inTexCoord * inTilingFactor); break;
    case 30: texColor *= texture(textures[30], inTexCoord * inTilingFactor); break;
    case 31: texColor *= texture(textures[31], inTexCoord * inTilingFactor); break;
  };
  
  if (texColor.a == 0.0) discard;

  if (!enableLighting) {
    fragColor = texColor;
    return;
  }

  vec3 result = vec3(0.0);

  for (int i = 0; i < noLights; i++)
    result += CalcPointLight(lights[i] , inNormal, inFragPos);

  fragColor = texColor * vec4(result, 1.f);
}
