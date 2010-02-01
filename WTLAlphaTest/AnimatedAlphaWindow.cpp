#include "StdAfx.h"
#include "AnimatedAlphaWindow.h"

using namespace Gdiplus;

void CAnimatedAlphaWindow::Update( )
{
	mGdiDrawer->Initialize(m_hWnd);
	mGdiDrawer->Update(mVar1);
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

	// Drawing methods
	mGdiDrawer->Initialize(m_hWnd);
}

void CAnimatedAlphaWindow::UpdateSize()
{
	mGdiDrawer->UpdateSize(CRect(mX, mY, mW, mH));
}