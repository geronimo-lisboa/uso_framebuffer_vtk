#include<iostream>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkCamera.h>

#include <boost/exception/all.hpp>

#include "myMapper.h"

#include <vtkDebugLeaks.h>

//class myRenderPassExperimento : public vtkRenderPass
//{
//public:
//	void Render(const vtkRenderState *s) override
//	{
//		glClearDepth(1.0f);									// Depth Buffer Setup
//		glEnable(GL_DEPTH_TEST); // enable depth-testing
//		glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
//
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//		const int propCount = s->GetPropArrayCount();
//		for (auto i = 0; i < propCount; i++)
//		{
//			s->GetPropArray()[i]->RenderOpaqueGeometry(s->GetRenderer()); //OpaqueGeometry(s->GetRenderer());
//		}
//	}
//	static myRenderPassExperimento* New()
//	{
//		return new myRenderPassExperimento();
//	}
//};


int main(int argc, char **argv){
	try{

		vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
		vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
		vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();

		renderWindow->SwapBuffersOn();
		renderWindow->AddRenderer(renderer);
		renderWindow->SetInteractor(renderWindowInteractor);


		vtkSmartPointer<myActor> mapper = vtkSmartPointer<myActor>::New();

		renderer->AddActor(mapper);
		renderer->ResetCamera();
		renderer->GetActiveCamera()->ParallelProjectionOn();
		renderer->GetActiveCamera()->Zoom(1.5);


		renderer->ResetCamera();
		renderWindow->Render();
		renderWindowInteractor->Start();
	}
	catch (boost::exception &ex)
	{
		std::cout << boost::diagnostic_information(ex) << std::endl;
	}
	catch(std::exception &ex)
	{
		std::cout << ex.what() << std::endl;
	}
	catch(...)
	{
		std::cout << "alguma exeção misteriosa aconteceu..." << std::endl;
	}

	vtkDebugLeaks::SetExitError(0);
	if (vtkDebugLeaks::PrintCurrentLeaks() != 0)
		std::cout << "tem leaks..." << std::endl;
	return EXIT_SUCCESS;
}