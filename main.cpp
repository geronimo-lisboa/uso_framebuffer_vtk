#include <vtkObjectFactory.h>
#include<iostream>
#include <string>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkCamera.h>

#include <boost/exception/all.hpp>

#include "myLightningTestActor.h"

#include <vtkDebugLeaks.h>

#include <vtkOpenGLRenderer.h>
#include <vtkRenderPass.h>
#include <vtkSequencePass.h>
#include <vtkCameraPass.h>
#include <vtkLightsPass.h>
#include <vtkDefaultPass.h>
#include <vtkRenderPassCollection.h>

#include <vtkFramebufferPass.h>
#include <vtkTextureObject.h>
#include <vtkPixelBufferObject.h>


#include <vtkInteractorStyleTrackballCamera.h>
#include <boost\signals2.hpp>//O header da boost. Esse header precisa que sua lib seja inclusa.
#include <vtkImageData.h>
#include <vtkImageImport.h>
#include <vtkPNGWriter.h>

////http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-14-render-to-texture/
////http://www.lighthouse3d.com/tutorials/opengl_framebuffer_objects/
////https://open.gl/framebuffers

enum MyRenderPassSwitchFramebuffer{FB_ON, FB_OFF};

/*-Replicar o que já funciona.
-Por o framebuffer pass e uma flag de ativação do framebuffer
-Capturar o resultado do framebuffer.*/

class MyRenderPass : public vtkRenderPass{
private:
	vtkSmartPointer<vtkCameraPass> cameraPass;
	vtkSmartPointer<vtkLightsPass> lightsPass;
	vtkSmartPointer<vtkDefaultPass> defaultPass;
	vtkSmartPointer<vtkRenderPassCollection> passes;
	vtkSmartPointer<vtkSequencePass> sequencePass;
	//O framebuffer pass, como o camera pass, tem o SetDelegate, que é passar pra ele quais renderpasses
	//farão as coisas para ele.
	vtkSmartPointer<vtkFramebufferPass> framebufferPass;
	MyRenderPass(){
		sequencePass = vtkSmartPointer<vtkSequencePass>::New();
		lightsPass = vtkSmartPointer<vtkLightsPass>::New();
		defaultPass = vtkSmartPointer<vtkDefaultPass>::New();
		passes = vtkSmartPointer<vtkRenderPassCollection>::New();
		cameraPass = vtkSmartPointer<vtkCameraPass>::New();
		framebufferPass = vtkSmartPointer<vtkFramebufferPass>::New();
		passes->AddItem(lightsPass);
		passes->AddItem(defaultPass);
		sequencePass->SetPasses(passes);
		cameraPass->SetDelegatePass(sequencePass);
		//O framebuffer pass vai receber todos os passes;
		framebufferPass->SetDelegatePass(cameraPass);
	}

public:
	static MyRenderPass* New();
	void Render(const vtkRenderState *s){
		cameraPass->Render(s);//Renderização para a tela
		framebufferPass->Render(s);//Renderização para o framebuffer
		vtkTextureObject* colorBuffer = framebufferPass->GetColorTexture();
		vtkPixelBufferObject* pbo = colorBuffer->Download();
		unsigned int dims[] = { 600,600 };
		vtkIdType increments[] = { 1, 600 };
		unsigned char *data = new unsigned char[600 * 600 * 4];
		pbo->Download2D(VTK_UNSIGNED_CHAR, data, dims, 4, increments);
		vtkSmartPointer<vtkImageImport> import = vtkSmartPointer<vtkImageImport>::New();
		import->SetDataOrigin(0, 0, 0);
		import->SetDataSpacing(1, 1, 1);
		import->SetDataExtent(0, 599, 0, 599, 0, 0);
		import->SetNumberOfScalarComponents(4);
		import->SetDataScalarTypeToUnsignedChar();
		import->SetDataExtentToWholeExtent();
		import->SetImportVoidPointer(data);
		import->Update();
		vtkImageData *i = import->GetOutput();
		vtkSmartPointer<vtkPNGWriter> writer = vtkSmartPointer<vtkPNGWriter>::New();
		writer->SetFileName("C:\\programacao\\fb.png");
		writer->SetInputConnection(import->GetOutputPort());
		writer->Write();
	}

	//Isso aqui é bem especifico do experimento, é pra tratar o keypress e fazer o switch
	void operator() (MyRenderPassSwitchFramebuffer ev)
	{
		std::cout << "YO" << std::endl;
	}
};


// Define interaction style
class KeyPressInteractorStyle : public vtkInteractorStyleTrackballCamera
{
public:
	boost::signals2::signal<void(MyRenderPassSwitchFramebuffer)> switchFramebufferSignal;

	static KeyPressInteractorStyle* New();
	vtkTypeMacro(KeyPressInteractorStyle, vtkInteractorStyleTrackballCamera);
	virtual void OnKeyPress()
	{
		// Get the keypress
		vtkRenderWindowInteractor *rwi = this->Interactor;
		std::string key = rwi->GetKeySym();
		if (key == "F" || key == "f")
		{
			switchFramebufferSignal(FB_ON);
		}
		// Forward events
		vtkInteractorStyleTrackballCamera::OnKeyPress();
	}
};

vtkStandardNewMacro(KeyPressInteractorStyle);
vtkObjectFactoryNewMacro(MyRenderPass);

int main(int argc, char **argv){
	try{
		vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
		vtkSmartPointer<MyRenderPass> myRendering = vtkSmartPointer<MyRenderPass>::New();
		vtkOpenGLRenderer::SafeDownCast(renderer)->SetPass(myRendering);
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
		mapperMassinha->SetData("C:\\teste\\estudo-framebuffer\\assets\\Massinha Teste.obj", "C:\\teste\\estudo-framebuffer\\assets\\teste_tex.png");
		mapperPlano->SetData("C:\\teste\\estudo-framebuffer\\assets\\plano.obj", "C:\\teste\\estudo-framebuffer\\assets\\teste_tex.png");
		#endif
		mapperMassinha->SetPosition(1, 0, 0);
		mapperPlano->SetPosition(-1, 0, 0);
		renderer->SetBackground(0.5, 0, 0);
		renderer->AddActor(mapperMassinha);
		renderer->AddActor(mapperPlano);
		renderWindow->SetSize(600, 600);
		renderer->ResetCamera();

		vtkCamera *cam = renderer->GetActiveCamera();
		renderer->GetActiveCamera()->ParallelProjectionOn();
		cam->SetClippingRange(0.0001, 10);
		cam->Dolly(2);
		cam->SetViewAngle(45);


		renderWindow->Render();

		//Bota o bagulho pra controlar a interação do usuário.
		vtkSmartPointer<KeyPressInteractorStyle> style = vtkSmartPointer<KeyPressInteractorStyle>::New();
		renderWindowInteractor->SetInteractorStyle(style);
		style->SetCurrentRenderer(renderer);

		style->switchFramebufferSignal.connect(boost::ref(*myRendering));

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