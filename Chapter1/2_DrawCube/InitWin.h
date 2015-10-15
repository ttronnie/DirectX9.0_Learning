#ifndef __INITWIN_H__
#define __INITWIN_H__

#include <Windows.h>
//#include<D3DX10math.h>
#include <d3dx9.h>

struct Vertex{
	float _x, _y, _z;
	Vertex(){}
	Vertex(float x, float y, float z){
		_x = x;
		_y = y;
		_z = z;
	}
	static const DWORD FVF;
};


namespace d3d9{
	static HWND MainWindowHandle = 0;

	bool InitWindowsApp(HINSTANCE hInstance,
		int showCmd,
		int width,
		int height);

	bool InitDX3D(D3DDEVTYPE deviceType,
		IDirect3DDevice9 **device,
		int width,
		int height);

	int Run(bool (*display)(float timeDelta));

	LRESULT CALLBACK WndProc(HWND hWnd,
		UINT msg,
		WPARAM wParam,
		LPARAM lParam);

	template<class T> void Release(T t){
		if (t){
			t->Release();
			t = 0;
		}
	}

	template<class T> void Delete(T t){
		if (t){
			delete t;
			t = 0;
		}
	}
}

#endif