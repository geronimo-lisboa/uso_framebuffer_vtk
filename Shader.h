#pragma once
#include <vtk_glew.h>
#include <map>
#include <string>
using namespace std;
/*
Prefiro usar */
class Shader
{
private:
	GLuint programId, vsId, fsId;
	map<string, GLuint> attributes;
	map<string, GLuint> uniforms;

	std::string ReadShaderFile(std::string path);
	//Extrai a lista de atributos e uniformes do programa dado. Os pares nome/localiza��o no shader s�o retornados
	//nos mapas passados como par�metro.
	void introspectProgram(GLuint programId, map<string, GLuint> &attributes, map<string, GLuint> &uniforms);
	//Gera o log do shader. A responsabilidade de deletar a string alocada � do invocador da fun��o.
	string GetShaderInfoLog(GLuint object, PFNGLGETSHADERIVPROC glGet__iv, PFNGLGETSHADERINFOLOGPROC glGet__InfoLog);
	//Cria um shader do tipo dado a partir do c�digo fonte dado.
	GLuint MakeShader(GLenum type, std::string source);
	//Cria o programa de shader a partir dos shaders dados.
	GLuint MakeProgram(GLuint vertex_shader, GLuint fragment_shader);
public:
	Shader(string vsPath, string fsPath);
	~Shader();
	GLuint GetUniform(string name);
	GLuint GetAttribute(string name);
	GLuint GetProgramId();
	void UseProgram();
};