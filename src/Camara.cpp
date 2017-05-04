

#include "Camara.h"


Camara::Camara(vec3 position, vec3 direction, GLfloat sensitivity, GLfloat fov)
{
	cameraPos = position;
	cameraFront = normalize(direction);

	Sensitivity = sensitivity;
	FOV = fov;

}

void Camara::DoMovement(GLFWwindow * window)
{
	GLfloat Deltatime = 0.0f;
	GLfloat Lastframe = 0.0f;

	GLfloat currentFrame = glfwGetTime();
	Deltatime = currentFrame - Lastframe;
	Lastframe = currentFrame;

	GLint Status;
	Status = glfwGetKey(window, GLFW_KEY_W);
	if (Status == GLFW_PRESS) {
		cameraPos -= cameraFront * Sensitivity * Deltatime;
	}

	Status = glfwGetKey(window, GLFW_KEY_S);
	if (Status == GLFW_PRESS) {
		cameraPos += cameraFront * Sensitivity * Deltatime;
	}

	Status = glfwGetKey(window, GLFW_KEY_A);
	if (Status == GLFW_PRESS) {
		cameraPos += normalize(cross(cameraFront, cameraUp)) * Sensitivity * Deltatime;
	}
				
	Status = glfwGetKey(window, GLFW_KEY_D);
	if (Status == GLFW_PRESS) {
		cameraPos -= normalize(cross(cameraFront, cameraUp)) * Sensitivity * Deltatime;
	}
	/*
	Status = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	if (Status == GLFW_PRESS) {
		//Camara::MouseMove(window,x,y);
	}

	 //ADELANTE
	Status = glfwGetKey(window, GLFW_KEY_S);
	if (Status == GLFW_PRESS) {
		cameraPos.z += cameraPos.z * Sensitivity;
	}
	//ATRAS
	Status = glfwGetKey(window, GLFW_KEY_D);
	if (Status == GLFW_PRESS) {
		cameraPos.z -= cameraPos.z * Sensitivity;
	}
	*/
}

void Camara::MouseMove(GLFWwindow * window, double xpos, double ypos)
{
	
	//REVISAR
	if (firstMouse)
	{
		LastMx = xpos;
		LastMy = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - LastMx;
	GLfloat yoffset = LastMy - ypos;

	LastMx = xpos;
	LastMy = ypos;


	xoffset *= Sensitivity;
	yoffset *= Sensitivity;

	YAW += xoffset;
	PITCH += yoffset;

	if (PITCH > 89.0f)
		PITCH = 89.0f;
	if (PITCH < -89.0f)
		PITCH = -89.0f;

	if (YAW > 89.0f)
		YAW = 89.0f;
	if (YAW < -89.0f)
		YAW = -89.0f;

	vec3 apuntado;
	apuntado.x = cos(radians(YAW)) * cos(radians(PITCH));
	apuntado.y = sin(radians(PITCH));
	apuntado.z = sin(radians(YAW)) * cos(radians(PITCH));
	cameraFront = normalize(apuntado);
}

void Camara::MouseScroll(GLFWwindow * window, double xScroll, double yScroll)
{
	if (FOV >= 1.0f && FOV <= 100.0f)
		FOV -= yScroll;
	if (FOV <= 1.0f)
		FOV = 1.0f;
	if (FOV >= 45.0f)
		FOV = 45.0f;

}

mat4 Camara::LookAt()
{
	vec3 worldUp = vec3(0.0f, 1.0f, 0.0f);

	vec3 cameraRight = normalize(cross(cameraFront, worldUp));
	cameraUp = cross(cameraFront, cameraRight);

	mat4 MatrixVector;
	//Es posen a la inversa perque glm o detecta al reves
	MatrixVector[0][0] = cameraRight.x;
	MatrixVector[1][0] = cameraRight.y;
	MatrixVector[2][0] = cameraRight.z;

	MatrixVector[0][1] = cameraUp.x;
	MatrixVector[1][1] = cameraUp.y;
	MatrixVector[2][1] = cameraUp.z;

	MatrixVector[0][2] = cameraFront.x;
	MatrixVector[1][2] = cameraFront.y;
	MatrixVector[2][2] = cameraFront.z;

	mat4 MatrixPosition;
	//Es posen a la inversa perque glm o detecta al reves
	MatrixPosition[3][0] = -cameraPos.x;
	MatrixPosition[3][1] = -cameraPos.y;
	MatrixPosition[3][2] = -cameraPos.z;

	mat4 MatrixResultado = MatrixVector * MatrixPosition;

	return MatrixResultado;
}

GLfloat Camara::GetFOV()
{
	
	return FOV;
}


Camara::~Camara()
{

}
