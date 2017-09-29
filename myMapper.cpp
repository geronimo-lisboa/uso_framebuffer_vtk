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
#include <vtkCubeSource.h>
#include <vtkPoints.h>
#include <vtkWeakPointer.h>
#include <vtkTriangleFilter.h>
#include <array>

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
	bounds = { { -1.0, 1.0, -1.0, 1.0, -1.0, 1.0 } };
	isSet = false;
}


myActor::~myActor(){
	glDeleteBuffers(1, &vertexesVbo);
	glDeleteVertexArrays(1, &vao);
	std::cout << __FUNCTION__ << std::endl;
}

void myActor::SetUp(){

#ifdef AVELL
	shader = std::make_unique<Shader>("C:\\programacao\\estudo-framebuffer\\vertesShader.glsl", "C:\\programacao\\estudo-framebuffer\\fragmentShader.glsl");
#endif
#ifdef MEDILAB
	shader = std::make_unique<Shader>("C:\\teste\\estudo-framebuffer\\vertesShader.glsl", "C:\\teste\\estudo-framebuffer\\fragmentShader.glsl");
#endif
	//Define os vertices
	std::array<GLfloat, 3> pt0 = { { 1, 1, 1 } };
	PushTuple<std::array<GLfloat, 3>, std::vector<GLfloat>>(pt0, vertexes);
	std::array<GLfloat, 3> pt1 = { { -1, 1, 1 } };
	PushTuple<std::array<GLfloat, 3>, std::vector<GLfloat>>(pt1, vertexes);
	std::array<GLfloat, 3> pt2 = { { 1, -1, 1 } };
	PushTuple<std::array<GLfloat, 3>, std::vector<GLfloat>>(pt2, vertexes);
	std::array<GLfloat, 3> pt3 = { { -1, -1, 1 } };
	PushTuple<std::array<GLfloat, 3>, std::vector<GLfloat>>(pt3, vertexes);
	std::array<GLfloat, 3> pt4 = { { 1, 1, -1 } };
	PushTuple<std::array<GLfloat, 3>, std::vector<GLfloat>>(pt4, vertexes);
	std::array<GLfloat, 3> pt5 = { { -1, 1, -1 } };
	PushTuple<std::array<GLfloat, 3>, std::vector<GLfloat>>(pt5, vertexes);
	std::array<GLfloat, 3> pt6 = { { 1, -1, -1 } };
	PushTuple<std::array<GLfloat, 3>, std::vector<GLfloat>>(pt6, vertexes);
	std::array<GLfloat, 3> pt7 = { { -1, -1, -1 } };
	PushTuple<std::array<GLfloat, 3>, std::vector<GLfloat>>(pt7, vertexes);
	//Define os indices das faces
	std::array<GLuint, 36> elementArray = { {0,1,2, 1,2,3, 1,5,3, 5,3,7, 5, 4, 7, 4, 7, 6, 4, 0, 6, 0, 6, 2, 4,0,5, 5,0,1,6,2,7,7,2,3} };
	PushTuple<std::array<GLuint, 36>, std::vector<GLuint>>(elementArray, indexes);

	vao = 0;//Cria o vertex array object e liga o buffer a ele
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	vertexesVbo = 0;//Cria o buffer dos vertices e passa os dados pra ele.
	vertexesVbo = CreateGLArrayBuffer<GLfloat>(vertexes);
	elementBuffer = 0;//cria os elementos
	elementBuffer = CreateGLElementBuffer<GLuint>(indexes);

	shader->UseProgram();
	GLuint vpLocation = shader->GetAttribute("vp");//ligação vao-shader
	glEnableVertexAttribArray(vpLocation);
	glBindBuffer(GL_ARRAY_BUFFER, vertexesVbo);
	glVertexAttribPointer(vpLocation, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glUseProgram(0);
	//FIM EXPERIMENTO

	isSet = true;
}

int myActor::RenderOpaqueGeometry(vtkViewport *view){
	if (!isSet)
	{
		SetUp();
	}
	else{
		glFrontFace(GL_CW);

		GLenum err = GL_NO_ERROR;
		//Montagem da matriz mvp e sua preparação pro shader
		vtkRenderer* ren = vtkRenderer::SafeDownCast(view);
		vtkSmartPointer<vtkMatrix4x4> projMat = ren->GetActiveCamera()->GetProjectionTransformMatrix(ren);
		vtkSmartPointer<vtkMatrix4x4> viewMat = ren->GetActiveCamera()->GetViewTransformMatrix();
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
		glUniformMatrix4fv(mvpLocation, 1, true, mvpData.data());//passa a mvp pro shader

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
		glDrawElements(GL_TRIANGLES, indexes.size(), GL_UNSIGNED_INT, (void*)0);
		//glDrawArrays(GL_TRIANGLES, 0, vertexes.size());

		err = glGetError();
		if (err != GL_NO_ERROR)
			BOOST_THROW_EXCEPTION(OpenGLException(err));

		//FIM EXPERIMENTO
	}
	return 1;
}

