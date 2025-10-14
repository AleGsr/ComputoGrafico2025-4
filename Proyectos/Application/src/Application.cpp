#include "Application.h"
#include "ShaderFuncs.h"

#include <iostream>

void Application::setup() 
{
	setUpGeometry();
	setUpProgram1();
	setUpProgram2();
	projection = glm::perspective(45.0f, 1024.0f / 768.f, 0.1f, 100.0f);

	//std::cout <<"setup()" << std::endl;
}

void Application::update() 
{
	//time += 0.001f;

	move += 0.1f* direction;
	eye = glm:: vec3(0.0f, 0.0f, 2.0f + cos(time));

	camera = glm::lookAt(eye, center, glm::vec3(0.0f, 1.0f, 0.0f));
	//std::cout << move <<" , " << direction << std::endl;
}

void Application::draw() 
{
	//Seleccionar programa (shaders)
	glUseProgram(ids["program2"]);

	//Pasar el resto de los parametros para el programa
	//glUniform1f(ids["time2"], time);
	glUniform1f(ids["move2"], move);

	glUniformMatrix4fv(ids["camera"], 1,GL_FALSE, & camera[0][0]);
	glUniformMatrix4fv(ids["projection"], 1,GL_FALSE, & projection[0][0]);

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


void Application::setUpProgram1()
{
	std::string VertexShader = fileToString("Shaders/VertexShader.glsl");
	std::string FragmentShader = fileToString("Shaders/FragmentShader.glsl");
	ids["program1"] = InitializeProgram(VertexShader, FragmentShader);
	//ids["time1"] = glGetUniformLocation(ids["program1"], "time");
	ids["move1"] = glGetUniformLocation(ids["program1"], "move");
}

void Application::setUpProgram2()
{
	std::string VertexShader = fileToString("Shaders/VertexCamera.glsl");
	std::string FragmentShader = fileToString("Shaders/FragmentShader.glsl");
	ids["program2"] = InitializeProgram(VertexShader, FragmentShader);
	//ids["time2"] = glGetUniformLocation(ids["program2"], "time");
	ids["move2"] = glGetUniformLocation(ids["program2"], "move");
	ids["camera"] = glGetUniformLocation(ids["program2"], "camera");
	ids["projection"] = glGetUniformLocation(ids["program2"], "projection");
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

void Application::keyCallback(int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	glfwSetWindowShouldClose(window, true);

	//teclas para mover
	if (key == GLFW_KEY_A && action == GLFW_RELEASE)
		SwapDirection();

	if (key == GLFW_KEY_D && action == GLFW_PRESS)
		SwapDirection();
}

void Application::SwapDirection()
{
	direction *=-1.0f ;
}


