#version 330 core

out vec4 fragColor;

in vec4 oColor;

void main() {
  fragColor = oColor;
}
