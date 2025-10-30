#pragma once
#include <vector>
#include <map>
#include <glad.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Application
{

private:

	std::map<std::string, GLuint> ids;
	float time{ 0.0f };
	float move{ 0.0f };
	void setUpGeometry();
	void setUpCube();

	void setUpProgram1();
	void setUpProgram2();

	glm::mat4 camera;
	glm::mat4 projection;
	glm::mat4 model;
	glm::vec3 eye{0.0f,0.0f,2.0f};
	glm::vec3 center{1.0f,1.0f,0.5f};




public:

	GLFWwindow* window;
	float direction{ 1.0f };

	//Rotar 

	float moveHorizontal ; //cuanto rota en x
	float moveVertical;  //cuanto rota en y


	//Variables de elementos del draw
	GLuint programToUse;
	GLuint geometryToUse;

	std::string typeGeometry;
	GLenum drawModes;  //Tipo de dibujo
	int vertexNumber;  //Numero de vertices

	int currentProgram = 1;  //ID del Programa actual
	int currentGeometry = 1;  //ID de la geometría actual


	//Escalar
	float zoom = 0.7f;
	float minScale = 0.5f;
	float maxScale = 5.0f;


	void setup();
	void update();
	void draw();

	

	std::string fileToString(const std::string& filename);


	void keyCallback(int key, int scancode, int action, int mods);
	void mouseCallback(double xpos, double ypos); //Regresa la posición del mouse
	void scrollCallback(double xoffset, double yoffset); //Regresa el valor del scroll
	void SwapDirection();


};