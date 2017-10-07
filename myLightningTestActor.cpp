#include "myLightningTestActor.h"
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
#include <vtkOBJReader.h>
#include "myObjReader.h"
#include <string>
#include <vtkPointData.h>
#include <vtkDataArray.h>
#include <vtkPoints.h>
#include <vtkLightCollection.h>
#include <vtkLight.h>
#include <vtkTransform.h>


vtkObjectFactoryNewMacro(myLightiningTestActor);

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

void myLightiningTestActor::SetData(std::string objPath, std::string texturePath)
{
	this->objPath = objPath;
	this->texturePath = texturePath;
	wasObjAndTexInformed = true;
}

myLightiningTestActor::myLightiningTestActor() {
	bounds = { { -1.0, 1.0, -1.0, 1.0, -1.0, 1.0 } };
	isSet = false;
	wasObjAndTexInformed = false;
	objPath = "";
	texturePath = "";
}


myLightiningTestActor::~myLightiningTestActor() {
	glDeleteBuffers(1, &vertexesVbo);
	glDeleteBuffers(1, &texCoordsVbo);
	glDeleteVertexArrays(1, &vao);
	std::cout << __FUNCTION__ << std::endl;
}

template<class Tipo> void PushDataArray(vtkSmartPointer<vtkDataArray> src, vector<Tipo> &vec)
{
	for (int i = 0; i<src->GetNumberOfTuples(); i++)
	{
		double* t = src->GetTuple(i);
		const int tuple_size = src->GetSize() / src->GetNumberOfTuples();
		for (int j = 0; j < tuple_size; j++)
		{
			vec.push_back(t[j]);
		}
	}
}

void myLightiningTestActor::SetUp() {
	if (!wasObjAndTexInformed) //Se não tem os dados necessários nem continua
		BOOST_THROW_EXCEPTION(exception("Não invocou SetData(), não tem os dados necessários pra funcionar."));
#ifdef AVELL
	shader = std::make_unique<Shader>("C:\\programacao\\estudo-framebuffer\\shaders\\lightining.vs",
		"C:\\programacao\\estudo-framebuffer\\shaders\\lightining.fs");
#endif
#ifdef MEDILAB
	shader = std::make_unique<Shader>("C:\\teste\\estudo-framebuffer\\shaders\\lightining.vs", "C:\\teste\\estudo-framebuffer\\shaders\\lightining.fs");
#endif

	vtkSmartPointer<vtkOBJReader> objImporter = vtkSmartPointer<vtkOBJReader>::New();
	objImporter->SetFileName(objPath.c_str());
	objImporter->Update();
	vtkSmartPointer<vtkPolyData> resultado = objImporter->GetOutput();
	vtkSmartPointer<vtkDataArray> resultadoTexCoords = resultado->GetPointData()->GetTCoords();
	int tcsz = resultadoTexCoords->GetSize();
	vtkSmartPointer<vtkDataArray> resultadoNormals = resultado->GetPointData()->GetNormals();
	int nsz = resultadoNormals->GetSize();
	vtkSmartPointer<vtkDataArray> resultadoPontos = resultado->GetPoints()->GetData();

	PushDataArray<GLfloat>(resultadoTexCoords, textureCoordinates);
	PushDataArray<GLfloat>(resultadoPontos, vertexes);
	PushDataArray<GLfloat>(resultadoNormals, normals);

	vao = 0;//Cria o vertex array object e liga o buffer a ele
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	vertexesVbo = 0;//Cria o buffer dos vertices e passa os dados pra ele.
	vertexesVbo = CreateGLArrayBuffer<GLfloat>(vertexes);
	texCoordsVbo = 0;
	texCoordsVbo = CreateGLArrayBuffer<GLfloat>(textureCoordinates);
	normalsVbo = 0;
	normalsVbo = CreateGLArrayBuffer<GLfloat>(normals);

	shader->UseProgram();
	GLuint positionLocation = shader->GetAttribute("position");//ligação vao-shader
	glEnableVertexAttribArray(positionLocation);
	glBindBuffer(GL_ARRAY_BUFFER, vertexesVbo);
	glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	GLuint textureCoordinateLocation = shader->GetAttribute("textureCoordinate");//ligação vao-shader
	glEnableVertexAttribArray(textureCoordinateLocation);
	glBindBuffer(GL_ARRAY_BUFFER, texCoordsVbo);
	glVertexAttribPointer(textureCoordinateLocation, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	GLuint normalLocation = shader->GetAttribute("normal");//ligação vao-shader
	glEnableVertexAttribArray(normalLocation);
	glBindBuffer(GL_ARRAY_BUFFER, normalsVbo);
	glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glUseProgram(0);
	//Leitura da textura e sua carga pra GPU
	vtkSmartPointer<vtkPNGReader> textureReader = vtkSmartPointer<vtkPNGReader>::New();
	textureReader->SetFileName(texturePath.c_str());
	if (textureReader->CanReadFile(texturePath.c_str()) != 3)
		BOOST_THROW_EXCEPTION(exception(("PNG inválido ou não encontrado: " + texturePath).c_str()));
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

std::array<float, 16> vtkMatrixToArray(vtkSmartPointer<vtkMatrix4x4> mat)
{
	std::array<float, 16> mvpData;//Tem que ser float, o opengl moderno aparentemente não aceita doubles nos uniforms
	for (int i = 0; i < 16; i++)
	{
		mvpData[i] = mat->GetElement(i / 4, i % 4);
	}
	return mvpData;
}

int myLightiningTestActor::RenderOpaqueGeometry(vtkViewport *view) {
	if (!isSet)
	{
		SetUp();
	}
	else {
		glFrontFace(GL_CW);
		glDisable(GL_CULL_FACE);


		GLenum err = GL_NO_ERROR;
		//Montagem da matriz mvp e sua preparação pro shader
		vtkRenderer* ren = vtkRenderer::SafeDownCast(view);
		vtkSmartPointer<vtkMatrix4x4> modelMat = vtkSmartPointer<vtkMatrix4x4>::New();
		if (IsIdentity)
			modelMat->Identity();
		else
		{
			vtkSmartPointer<vtkTransform> t = vtkSmartPointer<vtkTransform>::New();
			t->DeepCopy(this->Transform);//A Transform tem a rotação, seja via RotateX e RotateWXYZ
			t->Translate(this->GetPosition());
			t->Update();
			t->GetMatrix(modelMat);
		}

		vtkSmartPointer<vtkMatrix4x4> projMat = ren->GetActiveCamera()->GetProjectionTransformMatrix(ren);
		vtkSmartPointer<vtkMatrix4x4> viewMat = ren->GetActiveCamera()->GetViewTransformMatrix();


		std::array<float, 16> viewData = vtkMatrixToArray(viewMat);
		std::array<float, 16> modelData = vtkMatrixToArray(modelMat);
		std::array<float, 16> projData = vtkMatrixToArray(projMat);

		glBindVertexArray(vao);//Começa a usar o vartex array
		shader->UseProgram();//Começa a usar o shader
		//Passa os atributos do vao, como ligados na construção do objeto.
		GLuint positionLocation = shader->GetAttribute("position");
		glBindAttribLocation(shader->GetProgramId(), positionLocation, "position");
		GLuint textureCoordinateLocation = shader->GetAttribute("textureCoordinate");
		glBindAttribLocation(shader->GetProgramId(), textureCoordinateLocation, "textureCoordinate");
		GLuint normalLocation = shader->GetAttribute("normal");
		glBindAttribLocation(shader->GetProgramId(), normalLocation, "normal");
		//passa as matrizes
		GLuint modelMatrixLocation = shader->GetUniform("modelMatrix");
		glUniformMatrix4fv(modelMatrixLocation, 1, true, modelData.data());
		GLuint viewMatrixLocation = shader->GetUniform("viewMatrix");
		glUniformMatrix4fv(viewMatrixLocation, 1, true, viewData.data());
		GLuint projectionMatrixLocation = shader->GetUniform("projectionMatrix");
		glUniformMatrix4fv(projectionMatrixLocation, 1, true, projData.data());
		//passa a luz
		vtkLight* aLight = vtkLight::SafeDownCast( ren->GetLights()->GetItemAsObject(0) );
		array<float, 3>lightPos = { { aLight->GetPosition()[0], aLight->GetPosition()[1], aLight->GetPosition()[2] } };
		GLuint lightPositionLocation = shader->GetUniform("lightPosition");
		glUniform3fv(lightPositionLocation, 1, lightPos.data());
		//Passa as texturas.
		glActiveTexture(GL_TEXTURE0);
		GLuint textureLocation = shader->GetUniform("texUnit");
		glUniform1i(textureLocation, /*GL_TEXTURE*/0);
		glBindTexture(GL_TEXTURE_2D, textureId);

		glDrawArrays(GL_TRIANGLES, 0, vertexes.size());

		err = glGetError();
		if (err != GL_NO_ERROR)
			BOOST_THROW_EXCEPTION(OpenGLException(err));

		//FIM EXPERIMENTO
	}
	return 1;
}

