#include "StdAfx.h"
#include "D2DWICDrawer.h"

class RenderTargetDC {
	ID2D1GdiInteropRenderTarget* m_renderTarget;
	HDC m_dc;

public:
	RenderTargetDC(ID2D1GdiInteropRenderTarget* renderTarget)
		: m_renderTarget(renderTarget)
		, m_dc(0)
	{
		m_renderTarget->GetDC(
			D2D1_DC_INITIALIZE_MODE_COPY,
			&m_dc);

	}

	~RenderTargetDC() {
		RECT rect = {};
		m_renderTarget->ReleaseDC(&rect);
	}

	operator HDC() const {
		return m_dc;
	}
};

void CD2DWICDrawer::Initialize( HWND hwnd_I )
{
	mHwnd = hwnd_I;
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &mD2DFactory);
	mWICFactory.CoCreateInstance(CLSID_WICImagingFactory);
}

void CD2DWICDrawer::UpdateSize( WTL::CRect rect_I )
{
	mLocation = rect_I.TopLeft();

	mWICFactory->CreateBitmap(rect_I.Width(), rect_I.Height(),
		GUID_WICPixelFormat32bppPBGRA, WICBitmapCacheOnLoad, &mWICbmp);
}

void CD2DWICDrawer::Update( IUIAnimationVariable *var_I )
{
	CreateDeviceResources();
	mD2DRenderTarget->BeginDraw();

	// Do some drawing here
	mD2DRenderTarget->DrawEllipse(
		D2D1::Ellipse(
			D2D1::Point2F(500,500), 400,200),
		mBrush, 50);

	// Update the display
	{
		RenderTargetDC dc(mInteropTarget);
		UINT w,h;
		mWICbmp->GetSize(&w, &h);
		SIZE s = {  w,h };
		POINT p = {0};
		POINT loc = {mLocation.x, mLocation.y};
		BLENDFUNCTION bf = {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};
		BOOL b = ::UpdateLayeredWindow(mHwnd, NULL, &loc, &s, dc, &p, RGB(0,255,255), &bf, ULW_ALPHA);
		ATLTRACE("ULW returns %d, err=%x\n", b, GetLastError());
	}

	const HRESULT hr = mD2DRenderTarget->EndDraw();

	if (D2DERR_RECREATE_TARGET == hr) {
		DiscardDeviceResources();
	}
}

void CD2DWICDrawer::CreateDeviceResources()
{
	// D2D render target for bitmap
	const D2D1_PIXEL_FORMAT format = 
		D2D1::PixelFormat(
		DXGI_FORMAT_B8G8R8A8_UNORM,
		D2D1_ALPHA_MODE_PREMULTIPLIED);

	const D2D1_RENDER_TARGET_PROPERTIES properties = 
		D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_DEFAULT,
		format,
		0.0f, // default dpi
		0.0f, // default dpi
		D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE);
	mD2DRenderTarget.Release();
	mD2DFactory->CreateWicBitmapRenderTarget(mWICbmp, properties, &mD2DRenderTarget);
	mD2DRenderTarget.QueryInterface(&mInteropTarget);

	mBrush.Release();
	mD2DRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Red, 0.5), &mBrush);
}

void CD2DWICDrawer::DiscardDeviceResources()
{
	mD2DRenderTarget.Release();
	mInteropTarget.Release();
	mBrush.Release();
}