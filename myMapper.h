#pragma once
#include <vtkMapper.h>
#include <vtkRenderer.h>

class myMapper :public vtkMapper{
private:
	myMapper();
	virtual ~myMapper();
public:
	static myMapper* New();

	void Render(vtkRenderer* ren, vtkActor* a) override;
};