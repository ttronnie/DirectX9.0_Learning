#include"InitWin.h"

const DWORD Vertex::FVF = D3DFVF_XYZ;

bool d3d9::InitWindowsApp(HINSTANCE hInstance,
	int showCmd,
	int width,
	int height){
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;
	wc.lpfnWndProc = d3d9::WndProc;
	wc.hIcon = LoadIcon(0,IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.lpszMenuName = 0;
	wc.lpszClassName = "DrawWin";

	if (!RegisterClass(&wc)){
		::MessageBox(0,"Register WNDCLASS Failed.","Error",MB_OK);
		return false;
	}
	d3d9::MainWindowHandle = CreateWindow("DrawWin",
		"DrawWin",
		WS_OVERLAPPEDWINDOW,
		0,
		0,
		width,
		height,
		0,
		0,
		hInstance,
		0);
	if (!d3d9::MainWindowHandle){
		::MessageBox(0,"Create Window Failed.","Error",MB_OK);
		return false;
	}
	::ShowWindow(d3d9::MainWindowHandle, showCmd);
	::UpdateWindow(d3d9::MainWindowHandle);

	return true;
}

bool d3d9::InitDX3D(D3DDEVTYPE deviceType,
	IDirect3DDevice9 ** device,
	int width,
	int height){
	IDirect3D9 *d3d;
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (!d3d){
		::MessageBox(0, "Create Direct Failed.", "Error", MB_OK);
		return false;
	}
	//important
	D3DCAPS9 devCaps;
	d3d->GetDeviceCaps(D3DADAPTER_DEFAULT, deviceType, &devCaps);

	int vp = 0;
	if (devCaps.DevCaps&D3DDEVCAPS_HWTRANSFORMANDLIGHT){
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}
	//
	D3DPRESENT_PARAMETERS dParam;
	dParam.BackBufferWidth = width;
	dParam.BackBufferHeight = height;
	dParam.BackBufferFormat = D3DFMT_A8R8G8B8;
	dParam.BackBufferCount = 1;
	dParam.Windowed = true;
	dParam.hDeviceWindow = d3d9::MainWindowHandle;
	dParam.SwapEffect = D3DSWAPEFFECT_DISCARD;
	dParam.EnableAutoDepthStencil = true;
	dParam.AutoDepthStencilFormat = D3DFMT_D24S8;
	dParam.MultiSampleType = D3DMULTISAMPLE_NONE;
	dParam.MultiSampleQuality = 0;
	dParam.Flags = 0;
	dParam.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	dParam.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	HRESULT hr = 0;
	hr = d3d->CreateDevice(D3DADAPTER_DEFAULT, deviceType, d3d9::MainWindowHandle, vp, &dParam, device);
	if (FAILED(hr)){
		dParam.AutoDepthStencilFormat = D3DFMT_D16;
		hr = d3d->CreateDevice(D3DADAPTER_DEFAULT, deviceType, d3d9::MainWindowHandle, vp, &dParam, device);
		if (FAILED(hr)){
			d3d->Release();
			::MessageBox(0,"Create Device Failed.","Error",MB_OK);
			return false;
		}
	}
	d3d->Release();

	return true;
}

int d3d9::Run(bool(*display)(float timeDelta)){
	MSG msg;
	static float lastTime = (float)timeGetTime();
	while (true){
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)){
			if (msg.message == WM_QUIT)
				break;
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		else{
			float currTime = (float)timeGetTime();
			float timeDelta = (float)(currTime - lastTime)*0.001f;

			display(timeDelta);
			lastTime = currTime;
		}
	}
	return msg.wParam;
}

LRESULT CALLBACK d3d9::WndProc(HWND hWnd,
							   UINT msg,
							   WPARAM wParam,
							   LPARAM lParam){
	switch (msg){
	case WM_LBUTTONDOWN:
		::MessageBox(0,"Hello,DirectX","Hello",MB_OK);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			::DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;
	}
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}