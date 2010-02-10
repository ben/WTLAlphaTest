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

	void Update(double alpha_I, double sweep_I);
	void UpdateSize(WTL::CRect rect_I);
	
private:
	HWND mHwnd;
	std::auto_ptr<Gdiplus::Bitmap> mBackbuffer;
	WTL::CPoint mLocation;
};
