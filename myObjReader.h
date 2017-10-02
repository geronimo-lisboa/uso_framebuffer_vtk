#pragma once
#include <string>
#include <vector>
#include <vtk_glew.h>

class MyObjReader{
private:
	//Método para fazer push de coisas nos buffers
	template<class TTuple, class TVector> void PushTuple(const TTuple &t, TVector &v){
		for (unsigned int i = 0; i < t.size(); i++)
		{
			v.push_back(t[i]);
		}
	}
	std::vector<GLfloat> vertexBuffer, texCoordBuffer, normalBuffer;
public:
	void Read(std::string path);
	std::vector<GLfloat> GetVertexBuffer();
	std::vector<GLfloat> GetTexCoordBuffer();
	std::vector<GLfloat> GetNormalBuffer();
};