#include "Application.h"
#include "ShaderFuncs.h"

#include <iostream>

void Application::setup()
{
	//setUpGeometry();

	setUpCube();

	setUpProgram1();
	setUpProgram2();
	projection = glm::perspective(45.0f, 1024.0f / 768.f, 0.1f, 100.0f);

	//std::cout <<"setup()" << std::endl;
}

void Application::update()
{
	//time += 0.001f;

	move += 0.1f * direction;
	eye = glm::vec3(0.0f, 0.0f, 2.0f + cos(time));

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

	glUniformMatrix4fv(ids["camera"], 1, GL_FALSE, &camera[0][0]);
	glUniformMatrix4fv(ids["projection"], 1, GL_FALSE, &projection[0][0]);

	//Seleccionar la geometria (el triangulo)
	//glBindVertexArray(ids["triangle"]);
	glBindVertexArray(ids["cube"]);

	//glDraw()
	//glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDrawArrays(GL_TRIANGLES, 0, 36);

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
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	//Colores
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (const void*)(12 * sizeof(float)));
	glEnableVertexAttribArray(1);

}

void Application::setUpCube()
{
	//Dibujamos el cubo 36 vertices con 36 colores
	std::vector<float> geometry
	{
		//VERTICES
		//  X     Y     Z      W

		//Cara Frontal
		-1.0f, -1.0f, 1.0f, 1.0f, //VFrontal 1
		-1.0f,  1.0f, 1.0f, 1.0f, //VFrontal 2
		 1.0f, -1.0f, 1.0f, 1.0f, //VFrontal 3
		 1.0f,  1.0f, 1.0f, 1.0f, //VFrontal 4
		-1.0f,  1.0f, 1.0f, 1.0f, //VFrontal 5
		 1.0f, -1.0f, 1.0f, 1.0f, //VFrontal 6

		//Cara Atras
		-1.0f, -1.0f, -1.0f, 1.0f,  //VAtras 1
		-1.0f,  1.0f, -1.0f, 1.0f,  //VAtras 2
		 1.0f, -1.0f, -1.0f, 1.0f,  //VAtras 3
		 1.0f,  1.0f, -1.0f, 1.0f,  //VAtras 4
		-1.0f,  1.0f, -1.0f, 1.0f,  //VAtras 5
		 1.0f, -1.0f, -1.0f, 1.0f,  //VAtras 6

		//Cara Arriba
		-1.0f, 1.0f,  1.0f, 1.0f, //VArriba 1
		-1.0f, 1.0f, -1.0f, 1.0f, //VArriba 2
		 1.0f, 1.0f,  1.0f, 1.0f, //VArriba 3
		 1.0f, 1.0f, -1.0f, 1.0f, //VArriba 4
		-1.0f, 1.0f, -1.0f, 1.0f, //VArriba 5
		 1.0f, 1.0f,  1.0f, 1.0f, //VArriba 6

		//Cara Abajo
		-1.0f, -1.0f,  1.0f, 1.0f, //VAbajo 1
		-1.0f, -1.0f, -1.0f, 1.0f, //VAbajo 2
		 1.0f, -1.0f,  1.0f, 1.0f, //VAbajo 3
		 1.0f, -1.0f, -1.0f, 1.0f, //VAbajo 4
		-1.0f, -1.0f, -1.0f, 1.0f, //VAbajo 5
		 1.0f, -1.0f,  1.0f, 1.0f, //VAbajo 6

		//Cara Derecha
		1.0f, -1.0f,  1.0f, 1.0f, //VDerecha 1
		1.0f,  1.0f,  1.0f, 1.0f, //VDerecha 2
		1.0f, -1.0f, -1.0f, 1.0f, //VDerecha 3
		1.0f,  1.0f, -1.0f, 1.0f, //VDerecha 4
		1.0f,  1.0f,  1.0f, 1.0f, //VDerecha 5
		1.0f, -1.0f, -1.0f, 1.0f, //VDerecha 3

		//Cara Izquierda
		-1.0f, -1.0f,  1.0f, 1.0f, //VIzquierda 1
		-1.0f,  1.0f,  1.0f, 1.0f, //VIzquierda 2
		-1.0f, -1.0f, -1.0f, 1.0f, //VIzquierda 3
		-1.0f,  1.0f, -1.0f, 1.0f, //VIzquierda 4
		-1.0f,  1.0f,  1.0f, 1.0f, //VIzquierda 5
		-1.0f, -1.0f, -1.0f, 1.0f, //VIzquierda 6



		//COLORES
		// R      G      B     A

		//Cara Frontal Rojo
		  1.0f , 0.0f,  0.0f, 1.0f, //VF1
		  1.0f , 0.0f,  0.0f, 1.0f, //VF2
		  1.0f , 0.0f,  0.0f, 1.0f, //VF3
		  1.0f , 0.0f,  0.0f, 1.0f, //VF4
		  1.0f , 0.0f,  0.0f, 1.0f, //VF5
		  1.0f , 0.0f,  0.0f, 1.0f, //VF6		

		//Cara Atras Naranja/Morado
		  1.0f , 0.0f,  1.0f, 1.0f, //VAt1
		  1.0f , 0.0f,  1.0f, 1.0f, //VAt2
		  1.0f , 0.0f,  1.0f, 1.0f, //VAt3
		  1.0f , 0.0f,  1.0f, 1.0f, //VAt4
		  1.0f , 0.0f,  1.0f, 1.0f, //VAt5
		  1.0f , 0.0f,  1.0f, 1.0f, //VAt6

		//Cara Arriba Blanco
		  1.0f , 1.0f,  1.0f, 1.0f, //VArr1
		  1.0f , 1.0f,  1.0f, 1.0f, //VArr2
		  1.0f , 1.0f,  1.0f, 1.0f, //VArr3
		  1.0f , 1.0f,  1.0f, 1.0f, //VArr4
		  1.0f , 1.0f,  1.0f, 1.0f, //VArr5
		  1.0f , 1.0f,  1.0f, 1.0f, //VArr6		

		//Cara Abajo Amarillo
		  1.0f , 1.0f,  0.0f, 1.0f, //VAb1
		  1.0f , 1.0f,  0.0f, 1.0f, //VAb2
		  1.0f , 1.0f,  0.0f, 1.0f, //VAb3
		  1.0f , 1.0f,  0.0f, 1.0f, //VAb4
		  1.0f , 1.0f,  0.0f, 1.0f, //VAb5
		  1.0f , 1.0f,  0.0f, 1.0f, //VAb6		
		  
		//Cara Derecha Azul
		  0.0f , 0.0f,  1.0f, 1.0f, //VDer1
		  0.0f , 0.0f,  1.0f, 1.0f, //VDer2
		  0.0f , 0.0f,  1.0f, 1.0f, //VDer3
		  0.0f , 0.0f,  1.0f, 1.0f, //VDer4
		  0.0f , 0.0f,  1.0f, 1.0f, //VDer5
		  0.0f , 0.0f,  1.0f, 1.0f, //VDer6		
		  
		//Cara Izquierda Verde
		  0.0f , 1.0f,  0.0f, 1.0f, //VIz1
		  0.0f , 1.0f,  0.0f, 1.0f, //VIz2
		  0.0f , 1.0f,  0.0f, 1.0f, //VIz3
		  0.0f , 1.0f,  0.0f, 1.0f, //VIz4
		  0.0f , 1.0f,  0.0f, 1.0f, //VIz5
		  0.0f , 1.0f,  0.0f, 1.0f, //VIz6



	};


	//Crear VAO
	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	ids["cube"] = VAO;
	glBindVertexArray(VAO);


	//Crear VBO 
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);


	//Pasar arreglo de vertices
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(GLfloat) * geometry.size(),
		&geometry[0],
		GL_STATIC_DRAW);

	//vertices
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	//Colores
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (const void*)((36*6) * sizeof(float)));
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
	direction *= -1.0f;
}


