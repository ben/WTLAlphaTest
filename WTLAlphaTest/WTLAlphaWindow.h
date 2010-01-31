#pragma once
#include "atlframe.h"


class CWTLAlphaWindow;
typedef WTL::CFrameWindowImpl<CWTLAlphaWindow, CWindow, 
			ATL::CWinTraits< WS_POPUP , WS_EX_LAYERED > > CWTLAlphaWindow_base;

class CWTLAlphaWindow 
	: public CWTLAlphaWindow_base
{
public:
	DECLARE_FRAME_WND_CLASS(_T("WTL Alpha Window"), 0);

	BEGIN_MSG_MAP(CWTLAlphaWindow)
		MSG_WM_SHOWWINDOW(OnShowWindow)
		MSG_WM_DESTROY(OnDestroy)
		CHAIN_MSG_MAP(CWTLAlphaWindow_base)
	END_MSG_MAP()

protected:
	typedef boost::shared_ptr<Gdiplus::Bitmap> BitmapPtr;
	BitmapPtr mBackbuffer;

	virtual void Update()=0;
	LONG mX, mY, mW, mH;

private:
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnDestroy();
	void OnShowWindow(BOOL bShow, UINT nStatus);
	void RefreshSize();

};
