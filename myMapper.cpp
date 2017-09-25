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
	vertexShader = nullptr;
	fragmentShader = nullptr;
	shaderProgram = nullptr;
}


myActor::~myActor(){
	std::cout << __FUNCTION__ << std::endl;
}

void myActor::SetUp(){
	vertexShader = vtkSmartPointer<vtkShader>::New();

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