#ifndef __INITWIN_H__
#define __INITWIN_H__

#include <Windows.h>

static HWND MainWindowHandle = 0;

bool InitWindowsApp(HINSTANCE hInstance,int nShowCmd);

int Run();

LRESULT CALLBACK WndProc(HWND hWnd,
						 UINT msg,
						 WPARAM wParam,
						 LPARAM lParam);


#endif // !__INITWIN_H__
