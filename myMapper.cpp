#include "myMapper.h"
#include <iostream>
#include <vtkObjectFactory.h>
#include <vtkPolyData.h>
#include <vtkTriangleFilter.h>
#include <vtkCleanPolyData.h>
#include <vtkCubeSource.h>
#include <vtkPolyData.h>

vtkObjectFactoryNewMacro(myActor);

myActor::myActor(){
	std::cout << __FUNCTION__ << std::endl;
	bounds = { { -0.5, 0.5, -0.5, 0.5, -0.5, 0.5 } };
	isSet = false;
}


myActor::~myActor(){
	std::cout << __FUNCTION__ << std::endl;
}

void myActor::SetUp(){


	vtkSmartPointer<vtkCubeSource> cubo = vtkSmartPointer<vtkCubeSource>::New();
	cubo->SetXLength(1);
	cubo->SetYLength(1);
	cubo->SetZLength(1);
	cubo->SetCenter(0, 0, 0);
	cubo->Update();

	isSet = true;
}

int myActor::RenderVolumetricGeometry(vtkViewport *view){
	std::cout << __FUNCTION__ << std::endl;
	if (!isSet)
	{
		SetUp();
	}
	else{

	}
	return 1;
}

const std::string myActor::ReadShaderFile(std::string path)const
{
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(path.c_str(), std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}
	else
	{
		throw std::exception("arquivo não foi aberto");
	}
	std::cout << VertexShaderCode << std::endl;
	return VertexShaderCode;
}
