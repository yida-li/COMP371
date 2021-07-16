#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shaderClass.h"
#include "VAO.h"
#include "EBO.h"
#include "VBO.h"
#include <stb/stb_image.h>

using namespace std;


GLuint indices[] = {

		0,2,1,
		0,3,2


};

GLfloat vertices[] = {

	// cordinate  [open is botomrighttopleft]      RGBA colors                   [stb is toprightbotomleft]

		-0.5f,	-0.5f,	0.0f,				0.63f,0.65f,0.29f ,					0.0f,0.0f,							// lower left corner
		-0.5f,	0.5f,	0.0f,				0.65f,0.29f,0.29f,					0.0f,3.0f,                       // lower right corner
		0.5f,	0.5f,	0.0f,				 0.3f,0.65f,0.29f ,					3.0f,3.0f,							// upper corner
		0.5f,	-0.5f,	0.0f,				 0.51f, 0.29f, 0.65f	,		3.0f,0.0f						// inner left


};

int main()
{

	cout << "meow";

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* yida = glfwCreateWindow(800, 800, "Meow?", NULL, NULL);
	if (yida == NULL)
	{
		cout << "Failed to initialize this like a coen 317 microprocessor";
		glfwTerminate();
		return -1;
	}



	/*
	GLfloat vertices[] = {
		-0.5f,	-0.5f * float(sqrt(3)) / 3, 0.0f,     0.63f,0.65f,0.29f ,  //lower left corner    yellow 162, 168, 50
		0.5f,	-0.5f * float(sqrt(3)) / 3, 0.0f,		 	0.65f,0.29f,0.29f, // lower right corner    red 168, 50, 50
		0.0f,	0.5f * float(sqrt(3)) * 2 / 3, 0.0f,			0.3f,0.65f,0.29f ,// upper corner    green 78, 168, 50

		-0.25f, 0.5f * float(sqrt(3)) / 6, 0.0f,			0.3f,0.65f ,0.29f,	//inner left    teal 78, 168, 50
		0.25f, 0.5f * float(sqrt(3)) / 6, 0.0f,				0.29f, 0.38f,	0.65f ,// inner right       ocean 50, 66, 168
		0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f	,				0.51f, 0.29f,		0.65f //inner down    haze 131, 50, 168

	};
	*/

	// Hex stuff     yellow 162, 168, 50
	//				red 168, 50, 50
	//				green 78, 168, 50
	//				teal 78, 168, 50
	//				ocean 50, 66, 168
	//				haze 131, 50, 168


	glfwMakeContextCurrent(yida);
	gladLoadGL();
	glViewport(0, 0, 800, 800);
	Shader shaderProgram("default.vert", "default.frag");
	VAO VAO1;
	VAO1.Bind();
	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");





	// Texture
	int widthImg, heightImg, numColCh;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* bytes = stbi_load("cat.png", &widthImg, &heightImg, &numColCh, 0);
	GLuint texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(bytes);
	glBindTexture(GL_TEXTURE_2D, 0);



	GLuint tex0Uni = glGetUniformLocation(shaderProgram.ID, "tex0");
	shaderProgram.Activate();
	glUniform1i(tex0Uni, 0);












	glClearColor(0.65f, 0.19f, 0.36f, 1.0f);    // 0.65f, 0.19f, 0.36f,   Hex overdrive pink   168, 50, 94
	glClear(GL_COLOR_BUFFER_BIT);

	glfwSwapBuffers(yida);

	while (!glfwWindowShouldClose(yida))
	{
		glClearColor(0.65f, 0.19f, 0.36f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shaderProgram.Activate();
		glUniform1f(uniID, 0.5f);
		glBindTexture(GL_TEXTURE_2D, texture);

		VAO1.Bind();


		// 1. simple triangle
		// glDrawArrays(GL_TRIANGLES, 0, 3);

		// 2. 4 triangles
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(yida);

		glfwPollEvents();
	}

	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	glDeleteTextures(1, &texture);

	shaderProgram.Delete();

	glfwDestroyWindow(yida);
	glfwTerminate();

	return 0;
}