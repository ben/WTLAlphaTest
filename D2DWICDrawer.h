#pragma once
#include "idrawer.h"
#include "WedgeDrawer.h"

class CD2DWICDrawer : public IDrawer
{
public:
	void Initialize(HWND hwnd_I);
	void Update(double alpha_I, double sweep_I);

	void DrawWedgeAtAngle( double sweep_I, CComPtr<ID2D1PathGeometry> path, CComPtr<ID2D1RadialGradientBrush> areaBrush, CComPtr<ID2D1RadialGradientBrush> lineBrush );
	void UpdateSize(WTL::CRect rect_I);
	
private:
	void CreateDeviceResources();
	void DiscardDeviceResources();

	HWND mHwnd;
	WTL::CPoint mLocation;

	// Direct2D
	CComPtr<ID2D1Factory> mD2DFactory;
	CComPtr<ID2D1RenderTarget> mD2DRenderTarget;
	CComPtr<ID2D1GdiInteropRenderTarget> mInteropTarget;

	// WIC
	CComPtr<IWICImagingFactory> mWICFactory;
	CComPtr<IWICBitmap> mWICbmp;

	// DirectWrite
	CComPtr<IDWriteTextFormat> mDWTextFormat;

	// Custom classes
	CWedgeDrawer mWedge;
};
