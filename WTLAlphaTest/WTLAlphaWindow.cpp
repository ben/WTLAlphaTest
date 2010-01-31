#include "StdAfx.h"
#include "WTLAlphaWindow.h"

using namespace Gdiplus;

#define UPDATE_FROM_SYSMETRIC(var, param)			\
	do {														\
		int __temp__ = GetSystemMetrics(param);	\
		if (var != __temp__)								\
		{														\
			needNewBitmap = true;						\
		}														\
		var = __temp__;									\
	} while (0)



int CWTLAlphaWindow::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	mW = mH = 0;
	RefreshSize();
	return 0;
}


void CWTLAlphaWindow::Update()
{
	Draw(mBackbuffer);

	// Create a memory DC
	HDC screenDC = ::GetDC(NULL);
	WTL::CDC memDC;
	memDC.CreateCompatibleDC(screenDC);
	::ReleaseDC(NULL, screenDC);

	// Copy the input bitmap and select it into the DC
	WTL::CBitmap localBmp;
	mBackbuffer->GetHBITMAP(Color(0,0,0,0), &localBmp.m_hBitmap);
	HBITMAP hOldBmp = memDC.SelectBitmap(localBmp);

	// Update the display
	POINT p = {0};
	POINT loc = {mX, mY};
	SIZE s = { mW, mH };
	BLENDFUNCTION bf = {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};
	BOOL b = ::UpdateLayeredWindow(m_hWnd, NULL, &loc, &s, memDC, &p, RGB(0,255,255), &bf, ULW_ALPHA);
	//ATLTRACE("ULW returns %d, err=%x\n", b, GetLastError());

	// Cleanup
	memDC.SelectBitmap(hOldBmp);
}

void CWTLAlphaWindow::OnDestroy()
{
	PostQuitMessage(0);
}

void CWTLAlphaWindow::OnShowWindow( BOOL bShow, UINT nStatus )
{
	RefreshSize();
	Update();
	this->SetMsgHandled(FALSE);
}

void CWTLAlphaWindow::RefreshSize()
{
	bool needNewBitmap = false;
	UPDATE_FROM_SYSMETRIC(mX, SM_XVIRTUALSCREEN);
	UPDATE_FROM_SYSMETRIC(mY, SM_YVIRTUALSCREEN);
	UPDATE_FROM_SYSMETRIC(mW, SM_CXVIRTUALSCREEN);
	UPDATE_FROM_SYSMETRIC(mH, SM_CYVIRTUALSCREEN);

	ATLTRACE("Size: %d,%d - %d,%d\n", mX, mY, mW, mH);

	if (needNewBitmap)
	{
		mBackbuffer.reset(new Bitmap(mW, mH));
	}
}