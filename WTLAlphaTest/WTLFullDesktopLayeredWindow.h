#pragma once
#include "atlframe.h"


class CWTLFullDesktopLayeredWindow;
typedef WTL::CFrameWindowImpl<CWTLFullDesktopLayeredWindow, CWindow, 
			ATL::CWinTraits< WS_POPUP , WS_EX_LAYERED > > CWTLFullDesktopLayeredWindowBase;

class CWTLFullDesktopLayeredWindow 
	: public CWTLFullDesktopLayeredWindowBase
{
public:
	DECLARE_FRAME_WND_CLASS(_T("WTL Alpha Window"), 0);

	BEGIN_MSG_MAP(CWTLFullDesktopLayeredWindow)
		MSG_WM_SHOWWINDOW(OnShowWindow)
		MSG_WM_DESTROY(OnDestroy)
		CHAIN_MSG_MAP(CWTLFullDesktopLayeredWindowBase)
	END_MSG_MAP()

protected:
	LONG mX, mY, mW, mH;
	virtual void Update()=0;
	virtual void UpdateSize()=0;

private:
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnDestroy();
	void OnShowWindow(BOOL bShow, UINT nStatus);
	void RefreshSize();

};
