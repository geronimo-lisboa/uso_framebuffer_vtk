#include "myMapper.h"
#include <iostream>
#include <vtkObjectFactory.h>

vtkObjectFactoryNewMacro(myMapper);

myMapper::myMapper(){
	std::cout << __FUNCTION__ << std::endl;
}

myMapper::~myMapper(){
	std::cout << __FUNCTION__ << std::endl;
}

void myMapper::Render(vtkRenderer* ren, vtkActor* a){
	std::cout << __FUNCTION__ << std::endl;
}