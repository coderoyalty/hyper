#version 330 core

out vec4 fragColor;

in vec4 inColor;
in vec2 inTexCoord;

uniform sampler2D uFontAtlas;

float screenPxRange() {
    const float pxRange = 1.0; // set to distance field's pixel range
    vec2 unitRange = vec2(pxRange) / vec2(textureSize(uFontAtlas, 0));
    vec2 screenTexSize = vec2(1.0) / fwidth(inTexCoord);
    return max(0.5 * dot(unitRange, screenTexSize), 1.0);
}

float median(float c) {
    return c;
}

void main() {
    // Sample the single-channel texture
    float texValue = texture(uFontAtlas, inTexCoord).r;
    // Convert the single-channel value to RGB
    vec3 msd = vec3(texValue);

    // Compute signed distance
    float sd = median(msd.r);
    float screenPxDistance = screenPxRange() * (sd - 0.5);
    float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);

    if (opacity == 0.0)
        discard;

    vec4 bgColor = vec4(0.0);
    fragColor = mix(bgColor, inColor, opacity);
    if (fragColor.a == 0.0)
        discard;
}

