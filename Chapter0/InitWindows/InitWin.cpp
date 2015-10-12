#include "InitWin.h"

bool InitWindowsApp(HINSTANCE hInstance,
				    int nShowCmd){
	WNDCLASS wc;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	wc.lpszMenuName = 0;
	wc.lpszClassName = "InitWindow";
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	//register window class
	if (!RegisterClass(&wc)){
		::MessageBox(0, "Register Window Failed.", "Error", MB_OK);
		return false;
	}
	//create window class
	MainWindowHandle = ::CreateWindow("InitWindow",
		"InitWindow",
		WS_OVERLAPPEDWINDOW,
		0,
		0,
		800,
		600,
		0,
		0,
		hInstance,
		0);
	if (MainWindowHandle == 0){
		::MessageBox(0, "Create Window Failed.", "Error", MB_OK);
		return false;
	}

	ShowWindow(MainWindowHandle, nShowCmd);
	UpdateWindow(MainWindowHandle);

	return true;
}

int Run(){
	MSG msg;
	while (true){
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)){
			if (msg.message == WM_QUIT)
				break;
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		else
			::MessageBox(0,"lalaal",0,0);
	}

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam){
	switch (msg){
	case WM_LBUTTONDOWN:
		::MessageBox(0, "Hello,Windows.", "Init", MB_OK);
		return 0;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			::DestroyWindow(MainWindowHandle);
		return 0;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}