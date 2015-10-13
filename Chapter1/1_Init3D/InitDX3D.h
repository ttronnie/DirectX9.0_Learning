#ifndef __INITDX3D_H__
#define __INITDX3D_H__

#include<Windows.h>
#include<d3d9.h>

static HWND MainWindowHandle = 0;

bool InitWindowsApp(HINSTANCE hInstance, 
					int showCmd, 
					int width, int height);

bool InitDirect3D(D3DDEVTYPE deviceType,
				  IDirect3DDevice9 ** device,
				  int width,
				  int height);


int Run(bool(*display)(float deltaTime));

LRESULT CALLBACK WndProc(HWND hWnd,
						 UINT msg,
						 WPARAM wParam,
						 LPARAM lParam);

template<class T> void Release(T t)
{
	if (t)
	{
		t->Release();
		t = 0;
	}
}

template<class T> void Delete(T t)
{
	if (t)
	{
		delete t;
		t = 0;
	}
}
#endif