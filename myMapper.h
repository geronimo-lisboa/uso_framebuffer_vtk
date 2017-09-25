#pragma once
#include <vtkMapper.h>
#include <vtkRenderer.h>
/*
  1) Fazer o mapper desenhar algo.
  1.1) Ele está sendo invocado?
  1.2) Ele aceita comandos de opengl?
  1.3) Desenhar um triangulo pra ver o que está acontecendo.
  1.4) Usar a MVP do renderer.

  2) Fazer o renderer desenhar o cubo que eu preciso pra fazer o volume renderer
  2.1) Desenhar o cubo na tela.
  2.2) Desenhar segundo as regras do volume rendering.

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
class myMapper :public vtkMapper{
private:
	myMapper();
	virtual ~myMapper();
public:
	static myMapper* New();

	void Render(vtkRenderer* ren, vtkActor* a) override;
};