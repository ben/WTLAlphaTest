#include "StdAfx.h"
#include "AnimatedAlphaWindow.h"

#define SMALL 20
#define LARGE 100
 
#define ASSERT_SUCCEEDED(expr)						\
	do {														\
		HRESULT __hr__ = (expr);						\
		assert(SUCCEEDED(__hr__));						\
	} while (0)


CAnimatedAlphaWindow::CAnimatedAlphaWindow()
: mNextAlphaValue(SMALL)
, mGdiDrawer( new GdiplusDrawer() )
, mD2DWICDrawer( new CD2DWICDrawer() )
, mCurrentDrawer( NULL )
, mCurrentSweep( 45. )
{
#if 1
	mCurrentDrawer = (IDrawer *)mD2DWICDrawer.get();
#else
	mCurrentDrawer = (IDrawer *)mGdiDrawer.get();
#endif
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
	ASSERT_SUCCEEDED(mAnimTimer->SetFrameRateThreshold(300));

	// Create animation variable(s)
	ASSERT_SUCCEEDED(mAnimMgr->CreateAnimationVariable(LARGE, &mAlphaVar));
	ASSERT_SUCCEEDED(mAnimMgr->CreateAnimationVariable(mCurrentSweep, &mSweepVar));
}

void CAnimatedAlphaWindow::OnLButtonUp( UINT nFlags, CPoint point )
{
	// Create a storyboard
	CComPtr<IUIAnimationStoryboard> storyboard;
	ASSERT_SUCCEEDED(mAnimMgr->CreateStoryboard(&storyboard));

	// Create transitions
	CComPtr<IUIAnimationTransition> stopTransition;
	//ATLTRACE("Animating to %.0f\n", mNextAlphaValue);
	ASSERT_SUCCEEDED(mTransLib->CreateSmoothStopTransition(0.5,
		mNextAlphaValue, &stopTransition));
	mNextAlphaValue = (mNextAlphaValue == LARGE ? SMALL : LARGE);

	// Add the stopTransition
	ASSERT_SUCCEEDED(storyboard->AddTransition(mAlphaVar, stopTransition));

	// Schedule the storyboard to animate now
	UI_ANIMATION_SECONDS timeNow;
	ASSERT_SUCCEEDED(mAnimTimer->GetTime(&timeNow));
	ASSERT_SUCCEEDED(storyboard->Schedule(timeNow));
}

void CAnimatedAlphaWindow::OnKeyUp( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	if (nChar == 'A')
	{
		SwapSize();
	}

	if (nChar == ' ')
	{
		if (mCurrentDrawer == mD2DWICDrawer.get())
			mCurrentDrawer = (IDrawer *)mGdiDrawer.get();
		else
			mCurrentDrawer = (IDrawer *)mD2DWICDrawer.get();
		Update();
	}

	if (nChar == 'J' || nChar == 'K')
	{
		double newSweep = (nChar == 'K' ? mCurrentSweep + 15. : mCurrentSweep - 15.);
		AnimateSweepTo(newSweep);
		mCurrentSweep = newSweep;
	}
}

void CAnimatedAlphaWindow::UpdateSize()
{
	mGdiDrawer->Initialize(m_hWnd);
	mD2DWICDrawer->Initialize(m_hWnd);

	CRect r(mX, mY, mW, mH);
	mGdiDrawer->UpdateSize(r);
	mD2DWICDrawer->UpdateSize(r);
}

void CAnimatedAlphaWindow::Update( )
{
	double alpha, sweep;
	mAlphaVar->GetValue(&alpha);
	mSweepVar->GetValue(&sweep);
	mCurrentDrawer->Update(alpha, sweep);
}



////////////////////////////////////////////////////////////////////////////////
void CAnimatedAlphaWindow::SwapSize()
{
	// Switch size from main monitor to entire virtual desktop
	int mainScreenW = GetSystemMetrics(SM_CXSCREEN);
	if (mW == mainScreenW)
	{
		mX = GetSystemMetrics(SM_XVIRTUALSCREEN);
		mY = GetSystemMetrics(SM_YVIRTUALSCREEN);
		mW = GetSystemMetrics(SM_CXVIRTUALSCREEN);
		mH = GetSystemMetrics(SM_CYVIRTUALSCREEN);
	}
	else
	{
		mX = mY = 0;
		mW = GetSystemMetrics(SM_CXSCREEN);
		mH = GetSystemMetrics(SM_CYSCREEN);
	}
	ATLTRACE("Size: %d,%d - %d,%d\n", mX, mY, mW, mH);
	UpdateSize();
}

void CAnimatedAlphaWindow::AnimateSweepTo( double newSweep_I )
{
	// Create a storyboard
	CComPtr<IUIAnimationStoryboard> storyboard;
	ASSERT_SUCCEEDED(mAnimMgr->CreateStoryboard(&storyboard));

	// Create transitions
	CComPtr<IUIAnimationTransition> stopTransition;
	ASSERT_SUCCEEDED(mTransLib->CreateSmoothStopTransition(0.25,
		newSweep_I, &stopTransition));

	// Add the stopTransition
	ASSERT_SUCCEEDED(storyboard->AddTransition(mSweepVar, stopTransition));

	// Schedule the storyboard to animate now
	UI_ANIMATION_SECONDS timeNow;
	ASSERT_SUCCEEDED(mAnimTimer->GetTime(&timeNow));
	ASSERT_SUCCEEDED(storyboard->Schedule(timeNow));
}
