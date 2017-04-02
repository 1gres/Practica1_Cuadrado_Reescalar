
#pragma once
//GLEW
#define GLEW_STATIC
#include <GL\glew.h>
//GLFW
#include <GLFW\glfw3.h>
#include <iostream>

//Nuevas Librerias
#include <string>
#include <sstream>
#include <fstream>


class Shader
{
public:
	GLuint Program;
	Shader(const GLchar* VertexPath, const GLchar* FragmentPath);
	void USE();
	~Shader();

private:

};
