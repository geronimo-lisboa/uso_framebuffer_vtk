#include<iostream>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkCamera.h>

#include <boost/exception/all.hpp>

#include "myMapper.h"

int main(int argc, char **argv){
	try{
		vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
		vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
		vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();

		renderWindow->AddRenderer(renderer);
		renderWindow->SetInteractor(renderWindowInteractor);
		renderWindow->Render();

		vtkSmartPointer<myActor> mapper = vtkSmartPointer<myActor>::New();

		renderer->AddActor(mapper);
		renderer->ResetCamera();
		renderer->GetActiveCamera()->ParallelProjectionOn();
		renderer->GetActiveCamera()->Zoom(1.5);

		renderWindow->Render();

		renderWindowInteractor->Start();
	}
	catch (boost::exception &ex)
	{
		std::cout << boost::diagnostic_information(ex) << std::endl;
	}

	return EXIT_SUCCESS;
}