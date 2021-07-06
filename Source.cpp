#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
using namespace std;

int main()
{

	cout << "comp 371 openGL first tutorial";

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_ANY_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* yida = glfwCreateWindow(900, 700, "program 0000", NULL, NULL);
	if (yida == NULL)
	{
		cout << "Failed to initialize this like a coen 317 microprocessor";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(yida);

	gladLoadGL();
	glViewport(0, 0, 600, 600);
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);


	while (!glfwWindowShouldClose(yida))
	{
		glfwPollEvents();
	}


	glfwDestroyWindow(yida);
	glfwTerminate();

	return 0;

}