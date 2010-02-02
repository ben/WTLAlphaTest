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
{
	//mCurrentDrawer = (IDrawer *)mD2DWICDrawer.get();
	mCurrentDrawer = (IDrawer *)mGdiDrawer.get();
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
	for (int i=0; i<4; ++i)
	{
		CComPtr<IUIAnimationVariable> var;
		ASSERT_SUCCEEDED(mAnimMgr->CreateAnimationVariable(50, &var));
		mPosVars.push_back(var);
	}

	// Start the sinusoidals
	CComPtr<IUIAnimationStoryboard> storyboard;
	ASSERT_SUCCEEDED(mAnimMgr->CreateStoryboard(&storyboard));
	BOOST_FOREACH(CComPtr<IUIAnimationVariable> var, mPosVars)
	{
		CComPtr<IUIAnimationTransition> linear;
		CComPtr<IUIAnimationTransition> sinusoidal;
		UI_ANIMATION_SECONDS duration = ((DOUBLE)(rand()%100))/50;
		DOUBLE finalpos = (rand()%100)+300;
		DOUBLE period = ((DOUBLE)(rand()%100))/50;
		ASSERT_SUCCEEDED(mTransLib->CreateLinearTransition(duration, finalpos, &linear));
		ASSERT_SUCCEEDED(storyboard->AddTransition(var, linear));
		ASSERT_SUCCEEDED(mTransLib->CreateSinusoidalTransitionFromVelocity(200,
			period, &sinusoidal));
		ASSERT_SUCCEEDED(storyboard->AddTransition(var, sinusoidal));
	}
	UI_ANIMATION_SECONDS timeNow;
	ASSERT_SUCCEEDED(mAnimTimer->GetTime(&timeNow));
	ASSERT_SUCCEEDED(storyboard->Schedule(timeNow));
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
	if (nChar != ' ') return;

	if (mCurrentDrawer == mD2DWICDrawer.get())
		mCurrentDrawer = (IDrawer *)mGdiDrawer.get();
	else
		mCurrentDrawer = (IDrawer *)mD2DWICDrawer.get();
	Update();
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
	mCurrentDrawer->Update(mAlphaVar, mPosVars);
}


