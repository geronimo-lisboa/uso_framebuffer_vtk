#include "myMapper.h"
#include <iostream>
#include <vtkObjectFactory.h>
#include <vtkPolyData.h>
#include <vtkTriangleFilter.h>
#include <vtkCleanPolyData.h>
#include <vtkCubeSource.h>
#include <vtkPolyData.h>
#include <vector>

vtkObjectFactoryNewMacro(myActor);

myActor::myActor(){
	std::cout << __FUNCTION__ << std::endl;
	bounds = { { -0.5, 0.5, -0.5, 0.5, -0.5, 0.5 } };
	isSet = false;
}


myActor::~myActor(){
	std::cout << __FUNCTION__ << std::endl;
}

void myActor::SetUp(){
	shader = std::make_unique<Shader>("C:\\teste\\estudo-framebuffer\\vertesShader.glsl", "C:\\teste\\estudo-framebuffer\\fragmentShader.glsl");

	//EXPERIMENTO
	vertexes.push_back(-1.0f); vertexes.push_back(-1.0f); vertexes.push_back(0.0f);//Passa vertices pra ele
	vertexes.push_back(1.0f); vertexes.push_back(-1.0f); vertexes.push_back(0.0f);
	vertexes.push_back(-1.0f); vertexes.push_back(1.0f); vertexes.push_back(0.0f);
	vertexes.push_back(1.0f); vertexes.push_back(1.0f); vertexes.push_back(0.0f);
	vao = 0;//Cria o vertex array object e liga o buffer a ele
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	vertexesVbo = 0;//Cria o buffer dos vertices e passa os dados pra ele.
	glGenBuffers(1, &vertexesVbo);
	glBindBuffer(GL_ARRAY_BUFFER, vertexesVbo);
	glBufferData(GL_ARRAY_BUFFER, vertexes.size() * sizeof(float), vertexes.data(), GL_STATIC_DRAW);
	shader->UseProgram();
	GLuint vpLocation = shader->GetAttribute("vp");//ligação vao-shader
	glEnableVertexAttribArray(vpLocation);
	glBindBuffer(GL_ARRAY_BUFFER, vertexesVbo);
	glVertexAttribPointer(vpLocation, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glUseProgram(0);
	//FIM EXPERIMENTO

	isSet = true;
}

int myActor::RenderVolumetricGeometry(vtkViewport *view){
	std::cout << __FUNCTION__ << std::endl;
	if (!isSet)
	{
		SetUp();
	}
	else{
		//EXPERIMENTO
		shader->UseProgram();
		glBindVertexArray(vao);
		GLuint vpLocation = shader->GetAttribute("vp");
		glBindAttribLocation(shader->GetProgramId(), vpLocation, "vp");
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		//FIM EXPERIMENTO
	}
	return 1;
}

