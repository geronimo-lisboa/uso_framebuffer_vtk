#include<iostream>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkCamera.h>

#include <boost/exception/all.hpp>

#include "myLightningTestActor.h"

#include <vtkDebugLeaks.h>


int main(int argc, char **argv){
	try{
		vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
		vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
		vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();

		renderWindow->SwapBuffersOn();
		renderWindow->AddRenderer(renderer);
		renderWindow->SetInteractor(renderWindowInteractor);

		vtkSmartPointer<myLightiningTestActor> mapperMassinha = vtkSmartPointer<myLightiningTestActor>::New();
		vtkSmartPointer<myLightiningTestActor> mapperPlano = vtkSmartPointer<myLightiningTestActor>::New();
		#ifdef AVELL
		mapperMassinha->SetData("C:\\programacao\\estudo-framebuffer\\assets\\Massinha Teste.obj", "C:\\programacao\\estudo-framebuffer\\assets\\teste_tex.png");
		mapperPlano->SetData("C:\\programacao\\estudo-framebuffer\\assets\\plano.obj", "C:\\programacao\\estudo-framebuffer\\assets\\teste_tex.png");
		#endif
		#ifdef MEDILAB
		mapper->SetData("C:\\teste\\estudo-framebuffer\\assets\\Massinha Teste.obj", "C:\\teste\\estudo-framebuffer\\assets\\teste_tex.png");
		mapperPlano->SetData("C:\\teste\\estudo-framebuffer\\assets\\plano.obj", "C:\\teste\\estudo-framebuffer\\assets\\teste_tex.png");
		#endif
		mapperMassinha->SetPosition(0.5, 0, 0);
		mapperPlano->SetPosition(-0.5, 0, 0);
		renderer->SetBackground(0.5, 0, 0);
		renderer->AddActor(mapperMassinha);
		renderer->AddActor(mapperPlano);
		renderer->ResetCamera();
		//renderer->GetActiveCamera()->ParallelProjectionOn();
		//renderer->GetActiveCamera()->Zoom(5);

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