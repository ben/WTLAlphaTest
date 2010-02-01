// WTLAlphaTest.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "WTLAlphaTest.h"
#include "WTLFullDesktopLayeredWindow.h"
#include "AnimatedAlphaWindow.h"

WTL::CAppModule _Module;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	CoInitialize(NULL);

	ULONG_PTR token;
	{
		Gdiplus::GdiplusStartupInput gdisi;
		Gdiplus::Status s = Gdiplus::GdiplusStartup(&token, &gdisi, NULL);
		ATLTRACE("GDI+ init returned %d\n", s);
	}

	CAnimatedAlphaWindow wnd;
	wnd.Initialize();
	wnd.Create(NULL);
	wnd.ShowWindow(SW_SHOW);

	WTL::CMessageLoop loop;
	return loop.Run();
}

