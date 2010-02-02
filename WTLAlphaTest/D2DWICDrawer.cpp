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

	CComPtr<IDWriteFactory> dwFactory;
	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), 
		reinterpret_cast<IUnknown**>(&dwFactory));
	dwFactory->CreateTextFormat(L"Verdana", NULL, 
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		24.f,
		L"en-US",
		&mDWTextFormat);
	// H and V centering
	mDWTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	mDWTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
}

void CD2DWICDrawer::UpdateSize( WTL::CRect rect_I )
{
	mLocation = rect_I.TopLeft();

	mWICFactory->CreateBitmap(rect_I.Width(), rect_I.Height(),
		GUID_WICPixelFormat32bppPBGRA, WICBitmapCacheOnLoad, &mWICbmp);
}

void CD2DWICDrawer::CreateDeviceResources()
{
	if (mD2DRenderTarget) return;

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
}

void CD2DWICDrawer::DiscardDeviceResources()
{
	mD2DRenderTarget.Release();
	mInteropTarget.Release();
}

void CD2DWICDrawer::Update( IUIAnimationVariable *var_I, const IDrawer::VarVector &posVars_I )
{
	CreateDeviceResources();
	mD2DRenderTarget->BeginDraw();

	// Calculate ellipse dimensions
	UINT w,h;
	mWICbmp->GetSize(&w, &h);

	// Get value of animated variable
	double alphaD;
	var_I->GetValue(&alphaD);
	FLOAT alpha = (FLOAT)(alphaD / 100.);

	// Do some drawing here
	CComPtr<ID2D1SolidColorBrush> brush;
	mD2DRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Red, alpha), &brush);
	mD2DRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black, alpha/2));
	//mD2DRenderTarget->DrawEllipse(
	//	D2D1::Ellipse(
	//		D2D1::Point2F(cx,y),
	//		rx,ry),
	//	brush, (FLOAT)alphaD);
	FLOAT interval = (FLOAT)w / posVars_I.size();
	for (size_t i=0; i<posVars_I.size(); ++i)
	{
		DOUBLE y;
		posVars_I[i]->GetValue(&y);
		D2D1_ROUNDED_RECT r =
		{
			{ (FLOAT)i*10, (FLOAT)y, (FLOAT)(interval*(i+1) - interval/2)+100, (FLOAT)y+100 },
			10,10
		};
		mD2DRenderTarget->DrawRoundedRectangle(r, brush, (FLOAT)alphaD/2);
	}

	// Text label
	{
		std::wstring text = L"Direct2D / DirectWrite / WIC";
		D2D1_RECT_F textRect = {
			(FLOAT)mLocation.x, 
			(FLOAT)mLocation.y,
			(FLOAT)mLocation.x+w,
			(FLOAT)mLocation.y+h};
		mD2DRenderTarget->DrawText(text.c_str(), text.length(), mDWTextFormat, textRect, brush);
	}

	// Update the display
	{
		RenderTargetDC dc(mInteropTarget);
		SIZE s = {  w,h };
		POINT p = {0};
		POINT loc = {mLocation.x, mLocation.y};
		BLENDFUNCTION bf = {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};
		BOOL b = ::UpdateLayeredWindow(mHwnd, NULL, &loc, &s, dc, &p, RGB(0,255,255), &bf, ULW_ALPHA);
		//ATLTRACE("ULW returns %d, err=%x\n", b, GetLastError());
	}

	const HRESULT hr = mD2DRenderTarget->EndDraw();

	if (D2DERR_RECREATE_TARGET == hr) {
		DiscardDeviceResources();
	}
}

