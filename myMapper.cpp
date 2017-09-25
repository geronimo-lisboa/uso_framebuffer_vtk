#include "myMapper.h"
#include <iostream>
#include <vtkObjectFactory.h>

vtkObjectFactoryNewMacro(myActor);

myActor::myActor(){
	std::cout << __FUNCTION__ << std::endl;
	bounds = { { -0.5, 0.5, -0.5, 0.5, -0.5, 0.5 } };
}

myActor::~myActor(){
	std::cout << __FUNCTION__ << std::endl;
}

int myActor::RenderVolumetricGeometry(vtkViewport *view){
	std::cout << __FUNCTION__ << std::endl;
	return 1;
}