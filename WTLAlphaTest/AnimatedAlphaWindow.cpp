#include "StdAfx.h"
#include "AnimatedAlphaWindow.h"

#define ASSERT_SUCCEEDED(expr)						\
	do {														\
		HRESULT __hr__ = (expr);						\
		assert(SUCCEEDED(__hr__));						\
	} while (0)

void CAnimatedAlphaWindow::Update( )
{
	mGdiDrawer->Initialize(m_hWnd);
	mGdiDrawer->Update(mVar1);
}

void CAnimatedAlphaWindow::OnLButtonUp( UINT nFlags, CPoint point )
{
	// Create a storyboard
	CComPtr<IUIAnimationStoryboard> storyboard;
	ASSERT_SUCCEEDED(mAnimMgr->CreateStoryboard(&storyboard));

	// Create a transition
	CComPtr<IUIAnimationTransition> transition;
	ATLTRACE("Animating to %.0f\n", mNextAnimationValue);
	//mTransLib->CreateSmoothStopTransition(1.0,
	//	mNextAnimationValue, &transition);
	ASSERT_SUCCEEDED(mTransLib->CreateAccelerateDecelerateTransition(
							  0.5,
							  mNextAnimationValue,
							  0.2, 0.8, &transition));
	mNextAnimationValue = (mNextAnimationValue == 50. ? 20. : 50.);

	// Add the transition
	ASSERT_SUCCEEDED(storyboard->AddTransition(mVar1, transition));

	// Schedule the storyboard to animate now
	UI_ANIMATION_SECONDS timeNow;
	ASSERT_SUCCEEDED(mAnimTimer->GetTime(&timeNow));
	ASSERT_SUCCEEDED(storyboard->Schedule(timeNow));
}

void CAnimatedAlphaWindow::Initialize()
{
	// Initialize WAM objects
	ASSERT_SUCCEEDED(mAnimMgr.CoCreateInstance(CLSID_UIAnimationManager));
	ASSERT_SUCCEEDED(mAnimTimer.CoCreateInstance(CLSID_UIAnimationTimer));
	ASSERT_SUCCEEDED(mTransLib.CoCreateInstance(CLSID_UIAnimationTransitionLibrary)); 

	// Connect the animation manager to the timer
	// UI_ANIMATION_IDLE_BEHAVIOR_DISABLE tells the timer to shut itself off when there is nothing to
	// animate
	CComQIPtr<IUIAnimationTimerUpdateHandler> spTimerUpdateHandler(mAnimMgr);
	ASSERT_SUCCEEDED(mAnimTimer->SetTimerUpdateHandler(spTimerUpdateHandler,
		UI_ANIMATION_IDLE_BEHAVIOR_DISABLE));

	// Now connect the timer to this window
	CComObject<AnimTimerEventHandler> *evthandler;
	ASSERT_SUCCEEDED( CComObject<AnimTimerEventHandler>::CreateInstance(&evthandler));
	evthandler->Initialize(boost::bind(UpdateCallback, this));
	CComQIPtr<IUIAnimationTimerEventHandler> spTimerEventHandler(evthandler);
	ASSERT_SUCCEEDED(mAnimTimer->SetTimerEventHandler(spTimerEventHandler));

	// Create animation variable(s)
	ASSERT_SUCCEEDED(mAnimMgr->CreateAnimationVariable(50, &mVar1));

	// Drawing methods
	mGdiDrawer->Initialize(m_hWnd);
}

void CAnimatedAlphaWindow::UpdateSize()
{
	mGdiDrawer->UpdateSize(CRect(mX, mY, mW, mH));
}

CAnimatedAlphaWindow::CAnimatedAlphaWindow()
	: mNextAnimationValue(5.)
	, mGdiDrawer( new GdiplusDrawer() )
	, mD2DWICDrawer( new CD2DWICDrawer() )
{

}