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
/*
  1) Fazer o mapper desenhar algo.
  1.1) Ele está sendo invocado? - SIM
  1.2) Ele aceita comandos de opengl? - SIM
  1.3) Desenhar um triangulo pra ver o que está acontecendo. - SIM
  1.4) Usar a MVP do renderer.  - SIM

  2) Fazer o renderer desenhar o cubo que eu preciso pra fazer o volume renderer
  2.1) Desenhar o cubo na tela. - SIM
  2.2) Desenhar segundo as regras do volume rendering. - SIM

  3) O framebuffer
  3.1) Guardar o desenho do cubo no framebuffer, como falam para fazer com volume rendering.
  3.2) Mandar o desenho pra GPU, como pelo que eu entendi, é necessário.

  4) Carga da imagem
  4.1) Carregar a imagem
  4.2) Por na GPU como textura 3d

  5) Shader
  5.1) Escrever o shader de Volume Rendering.

  6) OpenCL
  6.1) Criar o contexto openCL.
  6.2) Fazer umas operações.
  6.3) Linkar a imagem no openCL à textura no openGL pra renderizar no VR.

  7) Conclusão
  7.1) Reescrever o código segundo as regras da VTK
  7.2) Tentar publicar na base de código da VTK
  7.3) Fazer uns artigos.
  */
class myActor :public vtkProp3D{
private:
	std::unique_ptr<Shader> shader;
	bool isSet;
	std::array<double, 6> bounds;
	std::vector<GLfloat> vertexes; //Estrutura pra guardar os vertices
	GLuint vao, vertexesVbo; //O vertex array object e o vertex buffer object.
	//Método para fazer push de coisas nos buffers
	template<class TTuple, class TVector> void PushTuple(const TTuple &t, TVector &v){
		for (unsigned int i = 0; i < t.size(); i++)
		{
			v.push_back(t[i]);
		}
	}
	//Pra criar as arrays de buffers
	template<class TType> GLuint CreateGLArrayBuffer(const std::vector<TType> &list){
		GLuint _buff = 0;
		glGenBuffers(1, &_buff);
		glBindBuffer(GL_ARRAY_BUFFER, _buff);
		glBufferData(GL_ARRAY_BUFFER, list.size() * sizeof(TType), list.data(), GL_STATIC_DRAW);
		return _buff;
	}
	//Pra criar a array de elementos - é um método separado pq não soube saber se era pra usar a GL_ELEMENT_ARRAY_BUFFER
	//ao inves da GL_ARRAY_BUFFER sem fazer gambiarra.
	template<class TType> GLuint CreateGLElementBuffer(const std::vector<TType> &list){
		GLuint _buff = 0;
		glGenBuffers(1, &_buff);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buff);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, list.size() * sizeof(TType), list.data(), GL_STATIC_DRAW);
		return _buff;
	}


	myActor();
	virtual ~myActor();
	void SetUp();

public:
	vtkTypeMacro(myActor, vtkProp3D);
	static myActor* New();

	double* GetBounds() override
	{
		return bounds.data();
	}

	int  RenderOpaqueGeometry(vtkViewport *view) override;
};