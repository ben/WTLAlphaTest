
#include "stdafx.h"
#include "WedgeDrawer.h"

CWedgeDrawer::CWedgeDrawer()
	: mSweep(0.)
	, mInnerRadius(50)
	, mOuterRadius(600)
	, mAlphaRadius(300)
	, mMarginThickness(2.)
{
}

void CWedgeDrawer::SetFactory(CComPtr<ID2D1Factory> fact_I)
{
	mFactory = fact_I;
	RegenerateGeometry();
}

void CWedgeDrawer::SetArcSweep(double newSweep_I)
{
	// Need more restrictions; <90 degrees?
	bool regenGeometry = (mSweep != newSweep_I);
	mSweep = newSweep_I;
	if (regenGeometry)
		RegenerateGeometry();
}

CComPtr<ID2D1RadialGradientBrush> CWedgeDrawer::LineBrush(CComPtr<ID2D1RenderTarget> rt_I)
{
	static const D2D1_GRADIENT_STOP stops[] =
	{
		{   0.f,  { 1., 1., 1., 1. }  },
		{	 0.5f, { 1., 1., 1., 1. }  },
		{   1.f,  { 1., 1., 1., 0. }  },
	};

	CComPtr<ID2D1GradientStopCollection> gradientStops;
	rt_I->CreateGradientStopCollection(
		stops,
		ARRAYSIZE(stops),
		&gradientStops
		);

	return BrushFromStops(rt_I, gradientStops);
}

CComPtr<ID2D1RadialGradientBrush> CWedgeDrawer::AreaBrush(CComPtr<ID2D1RenderTarget> rt_I)
{
	static const D2D1_GRADIENT_STOP stops[] =
	{
		{   0.f,  { 0., 0., 0., .75 }  },
		//{	 0.5f, { 0., 0., 0., .5 }  },
		{   1.f,  { 0., 0., 0., 0. }  },
	};

	CComPtr<ID2D1GradientStopCollection> gradientStops;
	rt_I->CreateGradientStopCollection(
		stops,
		ARRAYSIZE(stops),
		&gradientStops
		);

	return BrushFromStops(rt_I, gradientStops);
}


CComPtr<ID2D1RadialGradientBrush> CWedgeDrawer::BrushFromStops( CComPtr<ID2D1RenderTarget> rt_I, CComPtr<ID2D1GradientStopCollection> gradientStops ) 
{
	CComPtr<ID2D1RadialGradientBrush> brush;
	rt_I->CreateRadialGradientBrush(
		D2D1::RadialGradientBrushProperties(
			D2D1::Point2F(0, 0),
			D2D1::Point2F(0, 0),
			mAlphaRadius, mAlphaRadius),
		D2D1::BrushProperties(),
		gradientStops,
		&brush
		);
	return brush;
}

void CWedgeDrawer::RegenerateGeometry()
{
	// Points on horizontal line:
	// x^2 + y^2 = r^2      and     y = m
	// => x^2 = r^2 - m^2
	D2D1_POINT_2F p0 = D2D1::Point2F(
		(FLOAT)sqrt(mInnerRadius*mInnerRadius - mMarginThickness*mMarginThickness),
		(FLOAT)mMarginThickness);
	D2D1_POINT_2F p3 = D2D1::Point2F(
		(FLOAT)sqrt(mOuterRadius*mOuterRadius - mMarginThickness*mMarginThickness),
		(FLOAT)mMarginThickness);

	D2D1::Matrix3x2F rotation = D2D1::Matrix3x2F::Rotation((FLOAT)mSweep);
	D2D1_POINT_2F p1 = rotation.TransformPoint(
		D2D1::Point2F((FLOAT)mInnerRadius, -(FLOAT)mMarginThickness));
	D2D1_POINT_2F p2 = rotation.TransformPoint(
		D2D1::Point2F((FLOAT)mOuterRadius, -(FLOAT)mMarginThickness));

	// Points on other line:
	// x^2 + y^2 = r^2     &     y = mx + b
	// => m^2+1!=0 (i.e. angles not equal to 90 degrees)
	//    x = (-sqrt(-b^2+m^2 r^2+r^2)-b m)/(m^2+1)
	//    y = (b-m sqrt(-b^2+m^2 r^2+r^2))/(m^2+1)

	// Create the figure
	mGeometry.Release();
	mFactory->CreatePathGeometry(&mGeometry);
	{
		CComPtr<ID2D1GeometrySink> sink;
		mGeometry->Open(&sink);
		sink->BeginFigure(p0, D2D1_FIGURE_BEGIN_FILLED);
		sink->AddArc(D2D1::ArcSegment(p1, 
			D2D1::SizeF((FLOAT)mInnerRadius, (FLOAT)mInnerRadius), 0,
			D2D1_SWEEP_DIRECTION_CLOCKWISE, D2D1_ARC_SIZE_SMALL));
		sink->AddLine(p2);
		sink->AddArc(D2D1::ArcSegment(p3, 
			D2D1::SizeF((FLOAT)mOuterRadius, (FLOAT)mOuterRadius), 0,
			D2D1_SWEEP_DIRECTION_COUNTER_CLOCKWISE, D2D1_ARC_SIZE_SMALL));
		sink->EndFigure(D2D1_FIGURE_END_CLOSED);
		sink->Close();
	}
}

