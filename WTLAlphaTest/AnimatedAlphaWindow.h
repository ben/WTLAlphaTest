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
		MSG_WM_KEYUP(OnKeyUp)
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
	void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);



	////////////////////////////////////////////////////////////////////////////////
	void SwapSize();
	CComPtr<IUIAnimationManager>						mAnimMgr;
	CComPtr<IUIAnimationTimer>							mAnimTimer;
	CComPtr<IUIAnimationTransitionLibrary>			mTransLib;

	CComPtr<IUIAnimationVariable> mAlphaVar;
	IDrawer::VarVector mPosVars;

	boost::shared_ptr<AnimTimerEventHandler> mEventHandler;
	double mNextAlphaValue;

	IDrawer *mCurrentDrawer;
	std::auto_ptr<GdiplusDrawer> mGdiDrawer;
	std::auto_ptr<CD2DWICDrawer> mD2DWICDrawer;
};
