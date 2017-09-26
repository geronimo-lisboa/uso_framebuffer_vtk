#include "myMapper.h"
#include <iostream>
#include <vtkObjectFactory.h>
#include <vtkPolyData.h>
#include <vtkTriangleFilter.h>
#include <vtkCleanPolyData.h>
#include <vtkCubeSource.h>
#include <vtkPolyData.h>
#include <vector>
#include <vtkRenderer.h>
#include <vtkCamera.h>
#include <vtkMatrix4x4.h>
#include <boost/exception/all.hpp>
#include <boost/shared_ptr.hpp>
#include <sstream>
#include <gl/GLU.h>
#include <exception>

vtkObjectFactoryNewMacro(myActor);

class OpenGLException :public std::exception{
private:
	std::string msg;
public:
	OpenGLException(GLenum err){
		unsigned char* e = (unsigned char*)gluErrorString(err);
		std::string _se = reinterpret_cast<char*>(e);
		std::stringstream ss;
		ss << "Erro Opengl = " << err << " - " << _se;
		msg = ss.str();
	}
	virtual const char* what() const{
		return msg.c_str();
	}

};

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
		GLenum err = GL_NO_ERROR;
		//Montagem da matriz mvp e sua preparação pro shader
		vtkRenderer* ren = vtkRenderer::SafeDownCast(view);
		vtkMatrix4x4* projMat = ren->GetActiveCamera()->GetProjectionTransformMatrix(ren);
		vtkMatrix4x4* viewMat = ren->GetActiveCamera()->GetViewTransformMatrix();
		vtkSmartPointer<vtkMatrix4x4> vpMat = vtkSmartPointer<vtkMatrix4x4>::New();
		vtkMatrix4x4::Multiply4x4(projMat, viewMat, vpMat);
		std::array<float, 16> mvpData;//Tem que ser float, o opengl moderno aparentemente não aceita doubles nos uniforms
		for (int i = 0; i < 16; i++)
		{
			mvpData[i] = vpMat->GetElement(i / 4, i % 4);
		}
		glBindVertexArray(vao);//Começa a usar o vartex array
		shader->UseProgram();//Começa a usar o shader

		GLuint vpLocation = shader->GetAttribute("vp");//pega a localização da vertex position e faz o bind com o vao
		glBindAttribLocation(shader->GetProgramId(), vpLocation, "vp");
		GLuint mvpLocation = shader->GetUniform("mvp");
		glUniformMatrix4fv(mvpLocation, 1, false, mvpData.data());//passa a mvp pro shader
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		err = glGetError();
		if (err != GL_NO_ERROR)
			BOOST_THROW_EXCEPTION(OpenGLException(err));

		//FIM EXPERIMENTO
	}
	return 1;
}

