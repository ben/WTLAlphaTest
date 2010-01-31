#include "StdAfx.h"
#include "AnimatedAlphaWindow.h"

using namespace Gdiplus;

void CAnimatedAlphaWindow::Update( )
{
	if (!mVar1) return;

	{
		Graphics g(mBackbuffer.get());
		g.SetSmoothingMode(SmoothingModeHighQuality);
		g.Clear(Color(10,0,0,0));

		// Animated pen width
		double width;
		assert(SUCCEEDED(mVar1->GetValue(&width)));

		Pen p(Color(128,255,0,0), (REAL)width);
		g.DrawEllipse(&p, 100,100, 500,500);
	}

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

int CAnimatedAlphaWindow::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	return 0;
}

void CAnimatedAlphaWindow::OnLButtonUp( UINT nFlags, CPoint point )
{
	// Create a storyboard
	CComPtr<IUIAnimationStoryboard> storyboard;
	assert(SUCCEEDED(mAnimMgr->CreateStoryboard(&storyboard)));

	// Create a transition
	CComPtr<IUIAnimationTransition> transition;
	ATLTRACE("Animating to %.0f\n", mNextAnimationValue);
	//mTransLib->CreateSmoothStopTransition(1.0,
	//	mNextAnimationValue, &transition);
	assert(SUCCEEDED(mTransLib->CreateAccelerateDecelerateTransition(
							  0.5,
							  mNextAnimationValue,
							  0.2, 0.8, &transition)));
	mNextAnimationValue = (mNextAnimationValue == 50. ? 20. : 50.);

	// Add the transition
	assert(SUCCEEDED(storyboard->AddTransition(mVar1, transition)));

	// Schedule the storyboard to animate now
	UI_ANIMATION_SECONDS now;
	assert(SUCCEEDED(mAnimTimer->GetTime(&now)));
	assert(SUCCEEDED(storyboard->Schedule(now)));
}

void CAnimatedAlphaWindow::Initialize()
{
	// Initialize WAM objects
	assert(SUCCEEDED(mAnimMgr.CoCreateInstance(CLSID_UIAnimationManager)));
	assert(SUCCEEDED(mAnimTimer.CoCreateInstance(CLSID_UIAnimationTimer)));
	assert(SUCCEEDED(mTransLib.CoCreateInstance(CLSID_UIAnimationTransitionLibrary))); 

	// Connect the animation manager to the timer
	// UI_ANIMATION_IDLE_BEHAVIOR_DISABLE tells the timer to shut itself off when there is nothing to
	// animate
	CComQIPtr<IUIAnimationTimerUpdateHandler> spTimerUpdateHandler(mAnimMgr);
	assert(SUCCEEDED(mAnimTimer->SetTimerUpdateHandler(spTimerUpdateHandler,
		UI_ANIMATION_IDLE_BEHAVIOR_DISABLE)));

	// Now connect the timer to this window
	CComObject<AnimTimerEventHandler> *evthandler;
	assert(SUCCEEDED( CComObject<AnimTimerEventHandler>::CreateInstance(&evthandler)));
	evthandler->Initialize(boost::bind(UpdateCallback, this));
	CComQIPtr<IUIAnimationTimerEventHandler> spTimerEventHandler(evthandler);
	assert(SUCCEEDED(mAnimTimer->SetTimerEventHandler(spTimerEventHandler)));

	// Create animation variable(s)
	assert(SUCCEEDED(mAnimMgr->CreateAnimationVariable(50, &mVar1)));
}
