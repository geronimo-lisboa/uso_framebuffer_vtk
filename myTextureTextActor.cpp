#include "myTextureTextActor.h"
#include <iostream>
#include <vtkObjectFactory.h>
#include <vtkPolyData.h>
#include <vector>
#include <vtkRenderer.h>
#include <vtkCamera.h>
#include <vtkMatrix4x4.h>
#include <boost/exception/all.hpp>
#include <boost/shared_ptr.hpp>
#include <sstream>
#include <gl/GLU.h>
#include <array>
#include <vtkPNGReader.h>
#include "myObjReader.h"
#include <string>

vtkObjectFactoryNewMacro(myTextureTestActor);

class OpenGLException :public std::exception {
private:
	std::string msg;
public:
	OpenGLException(GLenum err) {
		unsigned char* e = (unsigned char*)gluErrorString(err);
		std::string _se = reinterpret_cast<char*>(e);
		std::stringstream ss;
		ss << "Erro Opengl = " << err << " - " << _se;
		msg = ss.str();
	}
	virtual const char* what() const {
		return msg.c_str();
	}

};

myTextureTestActor::myTextureTestActor() {
	bounds = { { -1.0, 1.0, -1.0, 1.0, -1.0, 1.0 } };
	isSet = false;
}


myTextureTestActor::~myTextureTestActor() {
	glDeleteBuffers(1, &vertexesVbo);
	glDeleteBuffers(1, &texCoordsVbo);
	glDeleteVertexArrays(1, &vao);
	std::cout << __FUNCTION__ << std::endl;
}

void myTextureTestActor::SetUp() {
	std::string textureFileName = "";
	std::string objectFileName = "";
#ifdef AVELL
	shader = std::make_unique<Shader>("C:\\programacao\\estudo-framebuffer\\shaders\\Texture2d.vs",
		"C:\\programacao\\estudo-framebuffer\\shaders\\Texture2d.fs");
	textureFileName = "C:\\programacao\\estudo-framebuffer\\assets\\teste_tex.png";
	objectFileName = "C:\\programacao\\estudo-framebuffer\\assets\\plano.obj";
#endif
#ifdef MEDILAB
	shader = std::make_unique<Shader>("C:\\teste\\estudo-framebuffer\\shaders\\Texture2d.vs", "C:\\teste\\estudo-framebuffer\\shaders\\Texture2d.fs");
	textureFileName = "C:\\teste\\estudo-framebuffer\\assets\\teste_tex.png";
	objectFileName = "C:\\teste\\estudo-framebuffer\\assets\\plano.obj";
#endif
	MyObjReader myReader;
	myReader.Read(objectFileName);
	vertexes = myReader.GetVertexBuffer();
	textureCoordinates = myReader.GetTexCoordBuffer();

	vao = 0;//Cria o vertex array object e liga o buffer a ele
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	vertexesVbo = 0;//Cria o buffer dos vertices e passa os dados pra ele.
	vertexesVbo = CreateGLArrayBuffer<GLfloat>(vertexes);
	texCoordsVbo = 0;
	texCoordsVbo = CreateGLArrayBuffer<GLfloat>(textureCoordinates);

	shader->UseProgram();
	GLuint vpLocation = shader->GetAttribute("vp");//ligação vao-shader
	glEnableVertexAttribArray(vpLocation);
	glBindBuffer(GL_ARRAY_BUFFER, vertexesVbo);
	glVertexAttribPointer(vpLocation, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	GLuint tcLocation = shader->GetAttribute("tc");//ligação vao-shader
	glEnableVertexAttribArray(tcLocation);
	glBindBuffer(GL_ARRAY_BUFFER, tcLocation);
	glVertexAttribPointer(tcLocation, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glUseProgram(0);
	//Leitura da textura e sua carga pra GPU
	vtkSmartPointer<vtkPNGReader> textureReader = vtkSmartPointer<vtkPNGReader>::New();
	textureReader->SetFileName(textureFileName.c_str());
	if (textureReader->CanReadFile(textureFileName.c_str()) != 3)
		BOOST_THROW_EXCEPTION(exception(("PNG inválido ou não encontrado: " + textureFileName).c_str()));
	textureReader->Update();
	texture = textureReader->GetOutput();
	texture->Print(std::cout);
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->GetDimensions()[0], texture->GetDimensions()[1] , 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->GetScalarPointer());
	glBindTexture(GL_TEXTURE_2D, 0);

	isSet = true;
}

int myTextureTestActor::RenderOpaqueGeometry(vtkViewport *view) {
	if (!isSet)
	{
		SetUp();
	}
	else {
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
		GLuint tcLocation = shader->GetAttribute("tc");//pega a localização da vertex position e faz o bind com o vao
		glBindAttribLocation(shader->GetProgramId(), tcLocation, "tc");

		GLuint mvpLocation = shader->GetUniform("mvp");
		glUniformMatrix4fv(mvpLocation, 1, true, mvpData.data());//passa a mvp pro shader

		glActiveTexture(GL_TEXTURE0);
		GLuint textureSamplerLocation = shader->GetUniform("texUnit");
		glUniform1i(textureSamplerLocation, /*GL_TEXTURE*/0);
		glBindTexture(GL_TEXTURE_2D, textureId);

		glDrawArrays(GL_TRIANGLES, 0, vertexes.size() / 3);

		err = glGetError();
		if (err != GL_NO_ERROR)
			BOOST_THROW_EXCEPTION(OpenGLException(err));

		//FIM EXPERIMENTO
	}
	return 1;
}

