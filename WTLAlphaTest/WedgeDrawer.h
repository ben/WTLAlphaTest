#pragma once

#include <d2d1.h>

class CWedgeDrawer
{
public:
	CWedgeDrawer();

	void SetFactory(CComPtr<ID2D1Factory> fact_I);
	void SetArcSweep(double newSweep_I);
	void Geometry(CComPtr<ID2D1PathGeometry> &path_O)
	{
		path_O = mGeometry;
	}

	CComPtr<ID2D1RadialGradientBrush> LineBrush(CComPtr<ID2D1RenderTarget> rt_I);
	CComPtr<ID2D1RadialGradientBrush> AreaBrush(CComPtr<ID2D1RenderTarget> rt_I);

private:
	CComPtr<ID2D1RadialGradientBrush> BrushFromStops( CComPtr<ID2D1RenderTarget> rt_I, CComPtr<ID2D1GradientStopCollection> gradientStops );
	void RegenerateGeometry();

	double mSweep;
	double mInnerRadius;
	double mOuterRadius;
	double mAlphaRadius;
	double mMarginThickness;
	CComPtr<ID2D1Factory> mFactory;
	CComPtr<ID2D1PathGeometry> mGeometry;
};
