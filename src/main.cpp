
#pragma once

//GLEW
#define GLEW_STATIC
#define PI 3.1415

#include <GL\glew.h>
//GLFW
#include <GLFW\glfw3.h>
#include <iostream>
#include "Shader.h"

//Nuevas Librerias
#include <string>
#include <sstream>
#include <fstream>

#include "SOIL.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

const GLint WIDTH = 800, HEIGHT = 600;
bool WIDEFRAME = false;
bool TeclaW = false;
bool TeclaUp = false;
bool TeclaDown = false;
bool TeclaLeft = false;
bool TeclaRight = false;
GLfloat valor;

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);


int main() {

	//initGLFW
	if (!glfwInit())
		exit(EXIT_FAILURE);

	//set GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	//create a window

	

	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);

	window = glfwCreateWindow(WIDTH, HEIGHT, "Practica Reescalado", nullptr, nullptr);
	if (!window) {
		cout << "Error al crear la ventana" << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);

	//set GLEW and inicializate
	

	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit()) {
		std::cout << "Error al inicializar glew" << std::endl;
		glfwTerminate();

	}

	//set function when callback
	glfwSetKeyCallback(window, key_callback);

	//set windows and viewport
	int screenWithd, screenHeight;
	glfwGetFramebufferSize(window, &screenWithd, &screenHeight);
	glViewport(0, 0, screenWithd, screenHeight);

	//cargamos los shader
	//GLuint programID = LoadShaders("./src/SimpleVertexShader.vertexshader", "./src/SimpleFragmentShader.fragmentshader");
	Shader move("./src/SimpleVertexShader.vertexshader", "./src/SimpleFragmentShader.fragmentshader");

	// Definir el buffer de vertices
	GLfloat vertexs[] =
	{
		//Positions			//colors           //Texture Coords 
		0.5f,  0.5f, 0.0f,		0.0f,0.1f,0.4f,  1.0f,1.0f,// Top Right
		0.5f, -0.5f, 0.0f,		0.0f,1.0f,0.0f,  1.0f,0.0f,// Bottom Right
		-0.5f, -0.5f, 0.0f,		0.0f,1.0f,0.0f,  0.0f,0.0f,// Bottom Left
		-0.5f,  0.5f, 0.0f,		0.0f,0.1f,0.4f,  0.0f,1.0f// Top Left 

	};

	// Definir el EBO 
	GLuint IndexBufferObject[]{

		0,1,2, //Triangle 1 Super Esquerra, Inferior Esquerra, Super Dret
		0,2,3  //Triangle 2 Super Esquerra, Inferior dret, Superior Dret
	};

	// Crear los VBO, VAO y EBO
	GLuint VBO;
	GLuint EBO;
	GLuint VAO;


	//reservar memoria para el VAO (VBO + EVO), VBO (Nuve de puntos de nuestro objeto (Posicionm, color, orden textura, indice)) y EBO (Triangularizacion (Vertice X e Y) (Grupos de 3))
	glGenBuffers(1, &VBO); //Pasa x numero de taquillas al vector VBO //glGenBuffer(Numbuffer, &VBO)
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);

	//Establecer el objeto
	//Enlazar el buffer con openGL
	glBindVertexArray(VAO);

	//Declarar el VBO y el EBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //Ordena la informacio per cada taquilla (Buffer)
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexs), vertexs, GL_STATIC_DRAW); //Li pases la informacio dels vertex
																			 //Variable(Variable especial per al VBO/EBO, tamany del que volem pasar, nom de la variable d'on passem el tamany, Dinamic/Statick -> Dibuixar)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(IndexBufferObject), IndexBufferObject, GL_STATIC_DRAW); //Li pases el ordre crear anteriorment

	// bufer de posicion																									 //Establecer las propiedades de los vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//bufer de color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//Buffer textura
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	//liberar el buffer de vertices
	glBindVertexArray(0);

	//Declaracio Textura
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int TexturaX, TexturaY;
	unsigned char* image = SOIL_load_image("./src/texture.png", &TexturaX, &TexturaY, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TexturaX, TexturaY, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	GLuint texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int TexturaX2, TexturaY2;
	unsigned char* image2 = SOIL_load_image("./src/texture2.png", &TexturaX2, &TexturaY2, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TexturaX2, TexturaY2, 0, GL_RGB, GL_UNSIGNED_BYTE, image2);
	SOIL_free_image_data(image2);
	glBindTexture(GL_TEXTURE_2D, 1);

	GLfloat Transparencia = 0.5;
	GLfloat x;
	

	//bucle de dibujado
	while (!glfwWindowShouldClose(window))
	{
		GLfloat x = valor;
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		//Establecer el color de fondo
		glClearColor(0.0, 1.0, 0.8, 1.0); //RGB + ALPHA
		glClear(GL_COLOR_BUFFER_BIT);

		//Carrega de texturas
		move.USE();

		mat4 transform;
		transform = translate(transform, vec3(0.5f, 0.5f, 0.0f));
				
		if (TeclaLeft) {
			
			transform = rotate(transform, x, vec3(0.0f, 0.0f, 1.0f));
		}

		if (TeclaRight) {
			
			transform = rotate(transform, x, vec3(0.0f, 0.0f, 1.0f));
		}

		transform = scale(transform, vec3(0.5f, 0.5f, 0.0f));

		
		
		GLint tranformLocation = glGetUniformLocation(move.Program, "transform");
		glUniformMatrix4fv(tranformLocation, 1, GL_FALSE, value_ptr(transform));
		

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(glGetUniformLocation(move.Program, "Texture1"), 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(move.Program, "Texture2"), 1);

		glUniform1f(glGetUniformLocation(move.Program, "Transparencia"), Transparencia);

		if (TeclaUp)
		{
			
			if (Transparencia <= 0) {
				Transparencia = 0; //controlem tranparencia maxima
			}
			else {
				Transparencia = Transparencia - 0.1;
			}
			glUniform1f(glGetUniformLocation(move.Program, "Transparencia"), Transparencia);
			TeclaUp = false;
		}

		if (TeclaDown)
		{
			if (Transparencia >= 1) {
				Transparencia = 1; //controlem tranparencia maxima
				
			}
			else {
				Transparencia = Transparencia + 0.1;
			}
			glUniform1f(glGetUniformLocation(move.Program, "Transparencia"), Transparencia);
			TeclaDown = false;

		}

		//pintar el VAO
		glBindVertexArray(VAO);

		//LINE: Linieas, FILL: COlor, POINT: Punts
		//pintar con lineas //Aqui pintaries les linies del triangle
		if (TeclaW) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		}
		else {
			//pintar con triangulos //Pintar la superficie (Amb el color)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// Swap the screen buffer
		glfwSwapBuffers(window);

	}
	// liberar la memoria de los VAO, EBO y VBO
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	//TODO, comprobar que la tecla pulsada es escape para cerrar la aplicación y la tecla w para cambiar a modo widwframe
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		if (TeclaW)
			TeclaW = false;
		else
			TeclaW = true;
	}

	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		if (TeclaUp)
			TeclaUp = false;
		else
			TeclaUp = true;
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
	{
		if (TeclaDown)
			TeclaDown = false;
		else
			TeclaDown = true;
	}
	if (key == GLFW_KEY_LEFT )
	{
		valor = valor + 0.1;
		TeclaLeft = true;
	}
	else
		TeclaLeft = false;

	if (key == GLFW_KEY_RIGHT )
	{
		valor = valor - 0.1;
		TeclaRight = true;	
	}
	else
		TeclaRight = false;
}
