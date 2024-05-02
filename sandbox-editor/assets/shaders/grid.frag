#version 330 core

layout(location = 1) in vec3 nearPoint;
layout(location = 2) in vec3 farPoint;
layout(location = 3) in mat4 fragViewProj;

out vec4 fragColor;


vec4 grid(vec3 fragPos3D, float scale) {
    // Calculate 2D coordinates for the grid pattern based on fragment's x and z coordinates
    vec2 coord = fragPos3D.xz * scale;

    // Calculate the derivative of the grid pattern to ensure consistent line width regardless of scale
    vec2 derivative = fwidth(coord);

    // Calculate the grid pattern based on the fractional part of the coordinates
    vec2 grid = abs(fract(coord - 0.5) - 0.5) / derivative;

    // Calculate the distance to the nearest grid line
    float line = min(grid.x, grid.y);

    // Calculate minimum derivative for both axes
    float minimumz = min(derivative.y, 1);
    float minimumx = min(derivative.x, 1);

    // Initialize the color with a default dark gray shade
    vec4 color = vec4(0.2, 0.2, 0.2, 1.0 - min(line, 1.0));

    // Highlight the z-axis grid lines
    if (fragPos3D.x > -0.1 * minimumx && fragPos3D.x < 0.1 * minimumx)
        color.z = 1.0;

    // Highlight the x-axis grid lines
    if (fragPos3D.z > -0.1 * minimumz && fragPos3D.z < 0.1 * minimumz)
        color.x = 1.0;

    return color;
}

float computeDepth(vec3 pos) {
    vec4 clip_space_pos = fragViewProj * vec4(pos.xyz, 1.0);
    return clip_space_pos.z / clip_space_pos.w;
}


float computeLinearDepth(vec3 pos) {
    float near = 0.01;
    float far = 1000.0;
    vec4 clip_space_pos = fragViewProj * vec4(pos.xyz, 1.0);
    float clip_space_depth = (clip_space_pos.z / clip_space_pos.w) * 2.0 - 1.0;
    float linearDepth = (2.0 * near * far) / (far + near - clip_space_depth * (far - near));
    return linearDepth;
}

void main()
{
	float t = -nearPoint.y / (farPoint.y - nearPoint.y);

	vec3 fragPos3D = nearPoint + t * (farPoint - nearPoint);

	float gridDepth = ((gl_DepthRange.diff * computeDepth(fragPos3D)) +
                gl_DepthRange.near + gl_DepthRange.far) / 2.0;

    float linearDepth = computeLinearDepth(fragPos3D);

    gl_FragDepth = gridDepth;
    float fading = max(0.0, 0.5 - linearDepth);

    fragColor = (grid(fragPos3D, 20) + grid(fragPos3D, 2)) * float(t > 0);
    fragColor.a *= fading;
}

