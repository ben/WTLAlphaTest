#include "StdAfx.h"
#include "GdiplusDrawer.h"
#include <atltypes.h>

using namespace Gdiplus;

void GdiplusDrawer::Initialize( HWND hwnd_I )
{
	mHwnd = hwnd_I;
}

void GdiplusDrawer::UpdateSize( WTL::CRect rect_I )
{
	mLocation = rect_I.TopLeft();
	mBackbuffer.reset(new Bitmap(rect_I.Width(), rect_I.Height()));
}

void GdiplusDrawer::Update( double /*alpha_I*/, double /*sweep_I*/ )
{
	//// Animated pen animVar
	//double animVar;
	//alpha_I->GetValue(&animVar);

	//{
	//	Graphics g(mBackbuffer.get());
	//	g.SetSmoothingMode(SmoothingModeHighQuality);
	//	g.SetCompositingQuality(CompositingQualityHighQuality);
	//	g.SetTextRenderingHint(TextRenderingHintAntiAlias);

	//	BYTE alpha = (BYTE)(255 * animVar / 100);
	//	g.Clear(Color(alpha/2,0,0,0));

	//	// Ellipse
	//	Pen p(Color(alpha,255,0,0), (REAL)animVar/2);
	//	//g.DrawEllipse(&p, 100,100, mBackbuffer->GetWidth()-200, mBackbuffer->GetHeight()-200);
	//	double interval = mBackbuffer->GetWidth() / sweep_I.size();
	//	double x;
	//	size_t i;
	//	for (x = interval/2, i=0;
	//		  i<sweep_I.size();
	//		  ++i, x += interval)
	//	{
	//		double y;
	//		HRESULT hr = sweep_I[i]->GetValue(&y);
	//		g.DrawEllipse(&p, (REAL)x, (REAL)y, 100.,100.);
	//	}

	//	// Label
	//	{
	//		std::wstring text = L"GDI+";
	//		StringFormat sf;
	//		sf.SetAlignment(StringAlignmentCenter);
	//		sf.SetLineAlignment(StringAlignmentCenter);
	//		Font f(L"Verdana", 24);
	//		SolidBrush br(Color(alpha, 255, 0, 0));
	//		g.DrawString(text.c_str(), text.length(), &f, 
	//			PointF((REAL)mBackbuffer->GetWidth()/2, (REAL)40),
	//			&br);
	//	}
	//}

	//// Create a memory DC
	//HDC screenDC = ::GetDC(NULL);
	//WTL::CDC memDC;
	//memDC.CreateCompatibleDC(screenDC);
	//::ReleaseDC(NULL, screenDC);

	//// Copy the input bitmap and select it into the DC
	//WTL::CBitmap localBmp;
	//mBackbuffer->GetHBITMAP(Color(0,0,0,0), &localBmp.m_hBitmap);
	//HBITMAP hOldBmp = memDC.SelectBitmap(localBmp);

	//// Update the display
	//POINT p = {0};
	//POINT loc = {mLocation.x, mLocation.y};
	//SIZE s = { mBackbuffer->GetWidth(), mBackbuffer->GetHeight() };
	//BLENDFUNCTION bf = {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};
	//BOOL b = ::UpdateLayeredWindow(mHwnd, NULL, &loc, &s, memDC, &p, RGB(0,255,255), &bf, ULW_ALPHA);
	////ATLTRACE("ULW returns %d, err=%x\n", b, GetLastError());

	//// Cleanup
	//memDC.SelectBitmap(hOldBmp);
}