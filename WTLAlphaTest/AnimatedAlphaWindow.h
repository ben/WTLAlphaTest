#pragma once
#include "WTLFullDesktopLayeredWindow.h"
#include "AnimationTimerEventHandler.h"
#include "GdiplusDrawer.h"
#include "D2DWICDrawer.h"

class CAnimatedAlphaWindow :
	public CWTLFullDesktopLayeredWindow
{
public:
	CAnimatedAlphaWindow();

	BEGIN_MSG_MAP(CAnimatedAlphaWindow)
		MSG_WM_LBUTTONUP(OnLButtonUp)
		CHAIN_MSG_MAP(CWTLFullDesktopLayeredWindow)
	END_MSG_MAP()

	void Initialize();

protected:
	typedef boost::shared_ptr<Gdiplus::Bitmap> BitmapPtr;

	void Update();
	void UpdateSize();

private:
	static void UpdateCallback(CAnimatedAlphaWindow *obj) { obj->Update(); }

	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnLButtonUp(UINT nFlags, CPoint point);

	CComPtr<IUIAnimationManager>						mAnimMgr;
	CComPtr<IUIAnimationTimer>							mAnimTimer;
	CComPtr<IUIAnimationTransitionLibrary>			mTransLib;

	CComPtr<IUIAnimationVariable> mVar1;

	boost::shared_ptr<AnimTimerEventHandler> mEventHandler;
	double mNextAnimationValue;

	std::auto_ptr<GdiplusDrawer> mGdiDrawer;
	std::auto_ptr<CD2DWICDrawer> mD2DWICDrawer;
};
