#pragma once
#include <vector>
#include <map>
#include <glad.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class Application
{

private:
	std::vector<float> geometry
	{  //  X     Y     Z      W
		-1.0f, 1.0f,  0.0f, 1.0f, //Vectice 1
		-1.0f, -1.0f, 0.0f, 1.0f, //Vectice 2
		1.0f,  -1.0f, 0.0f, 1.0f, //Vectice 3
	};

	std::map<std::string, GLuint> ids;


public:
	void setup();
	void update();
	void draw();
	void setUpGeometry();
	void setUpProgram();

	std::string fileToString(const std::string& filename);
};