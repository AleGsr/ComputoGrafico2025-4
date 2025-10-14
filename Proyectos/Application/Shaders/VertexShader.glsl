#version 460 core

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec4 vColor;

out vec4 vertexColor;
out vec4 vertexPosition;
uniform float time;
uniform float move;

void main()
{
	vertexColor = vColor;
	vec4 newPosition = vPosition;
	//Sumarle el valor desde c++
	//newPosition.x = newPosition.x * cos(time); //Modifica la posición de la figura
	newPosition.x = newPosition.x + move ; //Modifica la posición de la figura
	gl_Position = newPosition;  //equivale a hacer return gl_Position
}