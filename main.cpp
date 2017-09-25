#include<iostream>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkActor.h>

#include "myMapper.h"

int main(int argc, char **argv){
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();

	renderWindow->AddRenderer(renderer);
	renderWindow->SetInteractor(renderWindowInteractor);
	renderWindow->Render();

	vtkSmartPointer<myMapper> mapper = vtkSmartPointer<myMapper>::New();
	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);

	renderer->AddActor(actor);
	renderer->ResetCamera();
	renderWindow->Render();

	renderWindowInteractor->Start();

	return EXIT_SUCCESS;
}