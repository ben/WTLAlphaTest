#pragma once

#include <vector>
#include <uianimation.h>
#include <atltypes.h>

class IDrawer
{
public:
	virtual void Initialize(HWND hwnd_I)=0;
	virtual void Update(double alpha_I, double sweep_I)=0;
	virtual void UpdateSize(WTL::CRect rect_I)=0;
};
