#version 330 core

out vec4 fragColor;
out int entityId;

in vec4 oColor;
flat in int oEntityId;


void main() {
  fragColor = oColor;
  entityId = oEntityId;
}
