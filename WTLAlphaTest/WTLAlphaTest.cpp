// WTLAlphaTest.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "WTLAlphaTest.h"
#include "WTLAlphaWindow.h"
#include "AnimatedAlphaWindow.h"

WTL::CAppModule _Module;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	ULONG_PTR token;
	{
		Gdiplus::GdiplusStartupInput gdisi;
		Gdiplus::GdiplusStartup(&token, &gdisi, NULL);
	}
	CoInitialize(NULL);

	CAnimatedAlphaWindow wnd;
	wnd.Initialize();
	wnd.Create(NULL);
	wnd.ShowWindow(SW_SHOW);

	WTL::CMessageLoop loop;
	return loop.Run();

	//MSG msg;
	//while (GetMessage(&msg, NULL, 0, 0))
	//{
	//	TranslateMessage(&msg);
	//	DispatchMessage(&msg);
	//}
	//return msg.wParam;
}

