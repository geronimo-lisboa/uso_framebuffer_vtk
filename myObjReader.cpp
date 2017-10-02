#include "myObjReader.h"
#include <fstream>
#include <vector>
#include <boost/exception/all.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <array>
#include <iostream>
using namespace std;

void MyObjReader::Read(std::string path)
{
	//Leitura do arquivo
	ifstream  inFile;
	inFile.open(path);
	if (!inFile){
		BOOST_THROW_EXCEPTION(exception(("arquivo não encontrado: "+ path).c_str()));
	}
	vector<string> lineList;
	string currentLine;
	while (std::getline(inFile, currentLine))
	{
		lineList.push_back(currentLine);
	}
	inFile.close();
	//A informação das faces é da forma f x/y/z x/y/z x/y/z onde x é o vertice, y é a texture coordinate e z é a normal.
	//Os numeros são os indices nas listas de vértices, texture coordinates e normais.
	vector<array<float, 3>> vertexTable, normalTable;
	vector<array<float, 2>> texcoordTable;
	vector<array<unsigned int, 3>> faceDesc;
	for (string ln : lineList)
	{
		//split.
		vector<string> splittedString;
		boost::split(splittedString, ln, boost::is_any_of(" "));
		//ver qual é a informação
		if (splittedString[0] == "v")
		{
			array<float, 3> aVert = { { boost::lexical_cast<float>(splittedString[1]),
				boost::lexical_cast<float>(splittedString[2]),
				boost::lexical_cast<float>(splittedString[3]) } };
			vertexTable.push_back(aVert);
		}
		if (splittedString[0] == "vt")
		{
			array<float, 2> aTc = { { boost::lexical_cast<float>(splittedString[1]),
				boost::lexical_cast<float>(splittedString[2]) } };
			texcoordTable.push_back(aTc);
		}
		if (splittedString[0] == "vn")
		{
			array<float, 3> aN = { { boost::lexical_cast<float>(splittedString[1]),
				boost::lexical_cast<float>(splittedString[2]),
				boost::lexical_cast<float>(splittedString[3]) } };
			normalTable.push_back(aN);
		}
		if (splittedString[0] == "f")
		{
			vector<string> splittedFaceData;
			boost::split(splittedFaceData, splittedString[1], boost::is_any_of("/"));
			array<unsigned int, 3> aFaceData = { { boost::lexical_cast<unsigned int>(splittedFaceData[0]),
				boost::lexical_cast<unsigned int>(splittedFaceData[1]),
				boost::lexical_cast<unsigned int>(splittedFaceData[2]), } };
			faceDesc.push_back(aFaceData);
		}

		//guardar onde deve
	}
	//1)Montar a tabela de vértices
	//2)Montar a tabela de texture coordinates
	//3)Montar a tabela de normais
	//4)Percorrer a lista de faces e montar as faces.
	//5)Retornar tudo.
}