#pragma once
#include <vtkProp3D.h>
#include <vtkRenderer.h>
#include <array>
#include <vtkViewport.h>
#include <vtkSmartPointer.h>
#include <string>
#include "Shader.h"
#include <memory>
#include <vector>
#include <vtkPolyData.h>
#include <vtkImageData.h>
#include <vtkSmartPointer.h>

class myTextureTestActor :public vtkProp3D {
private:

	vtkSmartPointer<vtkImageData> texture;
	GLuint textureId;

	std::unique_ptr<Shader> shader;
	bool isSet;
	std::array<double, 6> bounds;
	std::vector<GLfloat> vertexes; //Estrutura pra guardar os vertices
	std::vector<GLfloat> textureCoordinates;//As coords de textura
	GLuint vao, vertexesVbo, texCoordsVbo; //O vertex array object e o vertex buffer object.
													  //Método para fazer push de coisas nos buffers
	template<class TTuple, class TVector> void PushTuple(const TTuple &t, TVector &v) {
		for (unsigned int i = 0; i < t.size(); i++)
		{
			v.push_back(t[i]);
		}
	}
	//Pra criar as arrays de buffers
	template<class TType> GLuint CreateGLArrayBuffer(const std::vector<TType> &list) {
		GLuint _buff = 0;
		glGenBuffers(1, &_buff);
		glBindBuffer(GL_ARRAY_BUFFER, _buff);
		glBufferData(GL_ARRAY_BUFFER, list.size() * sizeof(TType), list.data(), GL_STATIC_DRAW);
		return _buff;
	}
	//Pra criar a array de elementos - é um método separado pq não soube saber se era pra usar a GL_ELEMENT_ARRAY_BUFFER
	//ao inves da GL_ARRAY_BUFFER sem fazer gambiarra.
	template<class TType> GLuint CreateGLElementBuffer(const std::vector<TType> &list) {
		GLuint _buff = 0;
		glGenBuffers(1, &_buff);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buff);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, list.size() * sizeof(TType), list.data(), GL_STATIC_DRAW);
		return _buff;
	}


	myTextureTestActor();
	virtual ~myTextureTestActor();
	void SetUp();

public:
	vtkTypeMacro(myTextureTestActor, vtkProp3D);
	static myTextureTestActor* New();

	double* GetBounds() override
	{
		return bounds.data();
	}

	int  RenderOpaqueGeometry(vtkViewport *view) override;
};