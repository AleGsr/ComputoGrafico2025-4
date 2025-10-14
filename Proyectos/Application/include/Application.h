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
	
	std::map<std::string, GLuint> ids;


public:
	void setup();
	void update();
	void draw();
	void setUpGeometry();
	void setUpProgram();
	float time{ 0.0f };

	std::string fileToString(const std::string& filename);
};