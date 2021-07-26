#pragma once
#include "VAO.h"


VAO::VAO()
{

	glGenVertexArrays(1, &ID);

}


void VAO::LinkAttrib(VBO VBO, GLuint layout, GLuint numComponents, GLenum text, GLsizeiptr stride, void* offset) {

	VBO.Bind();
	glVertexAttribPointer(layout, numComponents, text, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	VBO.Unbind();




}


void VAO::Bind()
{

	glBindVertexArray(ID);

}

void VAO::Unbind()
{

	glBindVertexArray(0);

}
void VAO::Delete()
{

	glDeleteVertexArrays(1, &ID);

}