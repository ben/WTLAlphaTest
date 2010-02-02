#pragma once
#include "IDrawer.h"

class GdiplusDrawer : IDrawer
{
public:
	GdiplusDrawer()
		: mHwnd(NULL)
		, mBackbuffer()
	{}

	void Initialize(HWND hwnd_I);

	void Update(IUIAnimationVariable *var_I, const IDrawer::VarVector &posVars_I);
	void UpdateSize(WTL::CRect rect_I);
	
private:
	HWND mHwnd;
	std::auto_ptr<Gdiplus::Bitmap> mBackbuffer;
	WTL::CPoint mLocation;
};
