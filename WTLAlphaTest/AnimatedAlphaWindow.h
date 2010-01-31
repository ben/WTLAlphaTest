#pragma once
#include "wtlalphawindow.h"
#include "AnimationTimerEventHandler.h"

class CAnimatedAlphaWindow :
	public CWTLAlphaWindow
{
public:
	CAnimatedAlphaWindow()
		: mNextAnimationValue(50.)
	{}

	BEGIN_MSG_MAP(CAnimatedAlphaWindow)
		MSG_WM_CREATE(OnCreate)
		MSG_WM_LBUTTONUP(OnLButtonUp)
		CHAIN_MSG_MAP(CWTLAlphaWindow)
	END_MSG_MAP()

	void Initialize();

protected:
	void Draw(CWTLAlphaWindow::BitmapPtr surface_IO);

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
};
