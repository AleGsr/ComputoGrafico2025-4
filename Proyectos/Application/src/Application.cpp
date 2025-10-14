#include "Application.h"
#include "ShaderFuncs.h"

#include <iostream>

void Application::setup() 
{
	setUpGeometry();
	setUpProgram();

	//std::cout <<"setup()" << std::endl;
}

void Application::update() 
{
	time += 0.001f;
	//std::cout << "update()" << std::endl;
}

void Application::draw() 
{
	//Seleccionar programa (shaders)
	glUseProgram(ids["program"]);

	//Pasar el resto de los parametros para el programa
	glUniform1f(ids["time"], time);

	//Seleccionar la geometria (el triangulo)
	glBindVertexArray(ids["triangle"]);

	//glDraw()
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	//std::cout << "draw()" << std::endl;
}



void Application::setUpGeometry()
{

	std::vector<float> geometry
	{  //  X     Y     Z      W
		-1.0f, -1.0f, 0.0f, 1.0f, //Vectice 2
		-1.0f, 1.0f,  0.0f, 1.0f, //Vectice 1
		1.0f,  -1.0f, 0.0f, 1.0f, //Vectice 3
		1.0f,  1.0f,  0.0f, 1.0f, //vertice 4

		1.0f , 0.0f,  0.0f, 1.0f, //rojo
		0.0f , 1.0f,  0.0f, 1.0f, //verde
		0.0f , 0.0f,  1.0f, 1.0f, //azul
		0.0f , 1.0f,  0.0f, 1.0f, //verde
	};


	//Crear VAO
	GLuint VAO, VBO; //Cajota, canicas
	glGenVertexArrays(1, &VAO);
	ids["triangle"] = VAO;

	glBindVertexArray(VAO);//Hasta aquí ya se creó la caja



	//Crear VBO vertices
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO); //Esto todavía no ha reservado memoria	//Ya se crea la cajita de canicas



	//Pasar arreglo de vertices
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(GLfloat) * geometry.size(),  //calculo de tamaño en bytes
		&geometry[0],
		GL_STATIC_DRAW);  //Mandamos la geometria al buffer

	//vertices
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0,0);
	glEnableVertexAttribArray(0);

	//Colores
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (const void*)(12*sizeof(float)));
	glEnableVertexAttribArray(1);

}


void Application::setUpProgram()
{
	std::string FragmentShader = fileToString("Shaders/FragmentShader.glsl");
	std::string VertexShader = fileToString("Shaders/VertexShader.glsl");
	ids["program"] = InitializeProgram(VertexShader, FragmentShader);
	ids["time"] = glGetUniformLocation(ids["program"], "time");
}

std::string Application::fileToString(const std::string& filename)
{
	std::ifstream file(filename); //Abre archivo en modo entrada 


	//Verifica si el archivo se abrio bien
	if (!file.is_open()) {
		std::cerr << "Error: No se pudo abrir el archivo '" << filename << "'" << std::endl;
		return ""; // Retorna un string vacío en caso de error
	}


	std::stringstream buffer; //Crea un stream de string

	buffer << file.rdbuf(); //Lee el contenido del archivo al stringstream

	file.close(); //Cierra el archivo

	return buffer.str();  //Retorna el archivo resultante
}


