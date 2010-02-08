#pragma once

#include <d2d1.h>

class CWedgeDrawer
{
public:
	CWedgeDrawer()
		: mSweep(0.)
	{}

	void SetArcSweep(double newSweep_I)
	{
		mSweep = newSweep_I;
	}

	void Geometry(CComPtr<ID2D1PathGeometry> &path_O, CComPtr<ID2D1Factory> factory_I)
	{
		// Naive approach: generate when asked. Probably want to cache this.
		factory_I->CreatePathGeometry( &path_O );

		CComPtr<ID2D1PathGeometry> trianglePath;
		factory_I->CreatePathGeometry( &trianglePath );
		{
			CComPtr<ID2D1GeometrySink> triangleSink;
			trianglePath->Open(&triangleSink);

			// Draw from mathematical 0 (due east) to newSweep. The radius of the area drawn is 600.
			triangleSink->BeginFigure(D2D1::Point2F(600,0), D2D1_FIGURE_BEGIN_FILLED);
			triangleSink->AddLine(D2D1::Point2F(0,0));
			FLOAT angle_radians = mSweep * M_PI / 180.;
			D2D1_POINT_2F p = {
				600*cos(angle_radians),
				600*sin(angle_radians)};
			triangleSink->AddLine(p);
			triangleSink->EndFigure(D2D1_FIGURE_END_CLOSED);
			triangleSink->Close();
		}

		// Subtract out circle in center
		CComPtr<ID2D1EllipseGeometry> ellPath;
		factory_I->CreateEllipseGeometry(D2D1::Ellipse(D2D1::Point2F(0,0), 100,100), &ellPath);
		{
			CComPtr<ID2D1GeometrySink> outSink;
			path_O->Open(&outSink);
			trianglePath->CombineWithGeometry(ellPath, D2D1_COMBINE_MODE_EXCLUDE, NULL, outSink);
			outSink->Close();
		}

		// TODO: subtract out margins
	}

	CComPtr<ID2D1RadialGradientBrush> LineBrush(CComPtr<ID2D1RenderTarget> rt_I)
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

	CComPtr<ID2D1RadialGradientBrush> AreaBrush(CComPtr<ID2D1RenderTarget> rt_I)
	{
		static const D2D1_GRADIENT_STOP stops[] =
		{
			{   0.f,  { 0., 0., 0., .75 }  },
			{	 0.5f, { 0., 0., 0., .5 }  },
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

private:
	CComPtr<ID2D1RadialGradientBrush> BrushFromStops( CComPtr<ID2D1RenderTarget> rt_I, CComPtr<ID2D1GradientStopCollection> gradientStops ) 
	{
		CComPtr<ID2D1RadialGradientBrush> brush;
		rt_I->CreateRadialGradientBrush(
			D2D1::RadialGradientBrushProperties(
			D2D1::Point2F(0, 0),
			D2D1::Point2F(0, 0),
			400,400),
			D2D1::BrushProperties(),
			gradientStops,
			&brush
			);
		return brush;
	}

	double mSweep;
};
