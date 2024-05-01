#version 330 core

out vec4 fragColor;
out int entity;

in vec3 localPosition;
in vec4 color;
in float thickness;
in float fade;
flat in int entityId;

void main() {
	float distance = 1.0 - length(localPosition);
	float circle = smoothstep(0.0, fade, distance);
	circle *= smoothstep(thickness + fade, thickness, distance);

	if (circle == 0.0) discard;

	fragColor = color;

	fragColor.a *= circle;

	entity = entityId;
}
