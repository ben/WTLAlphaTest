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