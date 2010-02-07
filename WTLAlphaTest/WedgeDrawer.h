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

	void Geometry(CComPtr<ID2D1PathGeometry> &path_O)
	{
		// Naive approach: generate when asked. Probably want to cache this.
		CComPtr<ID2D1GeometrySink> sink;
		path_O->Open(&sink);

		// Draw from mathematical 0 (due east) to newSweep. The radius of the area drawn is 600.
		{
			D2D1_POINT_2F p = {600,0};
			sink->BeginFigure(p, D2D1_FIGURE_BEGIN_FILLED);
		}

		// Horizontal line to origin
		{
			D2D1_POINT_2F p = {0,0};
			sink->AddLine(p);
		}

		// Angled line to top of sweep area
		{
			// TODO: angle
			FLOAT angle_radians = mSweep * M_PI / 180.;
			D2D1_POINT_2F p = {
				600*sin(angle_radians),
				500*cos(angle_radians)};
			sink->AddLine(p);
		}
		sink->EndFigure(D2D1_FIGURE_END_CLOSED);

		// Subtract out circle in center
		{

		}

		// TODO: subtract out margins
		
		sink->Close();
	}

private:


	double mSweep;
};
