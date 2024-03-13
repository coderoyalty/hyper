#version 330 core
out vec4 FragColor;
in vec4 outColor;

uniform float redValue;

void main()
{
    FragColor = vec4(outColor);
    FragColor.r = redValue;
}
