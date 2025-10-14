#version 460 core

in vec4 vertexColor;
out vec4 outColor;
in vec4 vertexPosition;
uniform float time;
uniform float move;

void main()
{
	outColor = vertexColor;
}