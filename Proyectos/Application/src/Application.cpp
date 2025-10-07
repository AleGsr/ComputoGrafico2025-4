#include "Application.h"
#include <iostream>

void Application::setup() 
{
	//Crear VAO
	GLuint VAO, VBO; //Cajota, canicas
	glGenVertexArrays(1, &VAO);
	ids["triangle"] = VAO;

	glBindVertexArray(VAO);
	
	//Crear VBO vertices
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO); //Esto todavía no ha reservado memoria

	//Pasar arreglo de vertices
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(GLfloat) * geometry.size(),  //calculo de tamaño en bytes
		&geometry[0],
		GL_STATIC_DRAW);  //Mandamos la geometria al buffer

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);


	//std::cout <<"setup()" << std::endl;
}

void Application::update() 
{
	std::cout << "update()" << std::endl;
}

void Application::draw() 
{
	std::cout << "draw()" << std::endl;
}