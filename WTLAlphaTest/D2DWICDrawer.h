#pragma once
#include "idrawer.h"

class CD2DWICDrawer : public IDrawer
{
public:
	void Initialize(HWND hwnd_I);
	void Update(IUIAnimationVariable *var_I);
	void UpdateSize(WTL::CRect rect_I);
	
private:
	void CreateDeviceResources();
	void DiscardDeviceResources();

	HWND mHwnd;
	WTL::CPoint mLocation;

	CComPtr<ID2D1Factory> mD2DFactory;
	CComPtr<ID2D1RenderTarget> mD2DRenderTarget;
	CComPtr<ID2D1GdiInteropRenderTarget> mInteropTarget;
	CComPtr<ID2D1SolidColorBrush> mBrush;

	CComPtr<IWICImagingFactory> mWICFactory;
	CComPtr<IWICBitmap> mWICbmp;
};
