#include "InitDX3D.h"

bool InitWindowsApp(HINSTANCE hInstance,
	int showCmd,
	int width,
	int height){
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;
	wc.lpfnWndProc = (WNDPROC)WndProc;  //change 3添加类型
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.lpszMenuName = 0;
	wc.lpszClassName = "InitDX3D";

	if (!RegisterClass(&wc)){
		::MessageBox(0, "Init DX3D Failed.", "Error", MB_OK);
		return false;
	}

	MainWindowHandle = ::CreateWindow("InitDX3D",
		"InitDX3D",
		WS_OVERLAPPEDWINDOW,
		0,
		0,
		width,
		height,
		0,
		0,
		hInstance,
		0);
	if (!MainWindowHandle){
		::MessageBox(0, "Create Window Failed.", 0, 0);
		return false;
	}
	//CHANGE 1
	::ShowWindow(MainWindowHandle, SW_SHOW);
	::UpdateWindow(MainWindowHandle);

	return true;
}

//IDirect3DDevice9参数必须是指针的指针
bool InitDirect3D(D3DDEVTYPE deviceType,
				  IDirect3DDevice9 ** device,
				  int width,
				  int height){
	//get IDirect3D9 director
	IDirect3D9 * d3d9 = 0;
	d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
	if (!d3d9){
		::MessageBox(0, "Direct3DCreate9 Failed", "Error", MB_OK);
		return false;
	}
	//check device capacity
	D3DCAPS9 d3dcaps;
	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT,deviceType,&d3dcaps);
	int vp = 0;
	if (d3dcaps.DevCaps&D3DDEVCAPS_HWTRANSFORMANDLIGHT){
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else{
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}
	//init D3DPRESENT_PARAMETER
	D3DPRESENT_PARAMETERS dParam ;
	dParam.BackBufferWidth = width;
	dParam.BackBufferHeight = height;
	dParam.BackBufferFormat = D3DFMT_A8R8G8B8;
	dParam.BackBufferCount = 1;
	dParam.MultiSampleType = D3DMULTISAMPLE_NONE;
	dParam.MultiSampleQuality = 0;
	dParam.SwapEffect = D3DSWAPEFFECT_DISCARD;
	dParam.Windowed = true;
	dParam.hDeviceWindow = MainWindowHandle;
	dParam.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	dParam.EnableAutoDepthStencil = true;
	dParam.AutoDepthStencilFormat = D3DFMT_D24S8;
	dParam.Flags = 0;
	dParam.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

	//create IDirect3DDevice9 by D3DPRESENT_PARAMETERS
	HRESULT hr = 0;
	//device类型必须是双指针类型
	hr = d3d9->CreateDevice(D3DADAPTER_DEFAULT,deviceType,MainWindowHandle,vp,&dParam,device);
	if (FAILED(hr)){
		dParam.AutoDepthStencilFormat = D3DFMT_D16;
		hr = d3d9->CreateDevice(D3DADAPTER_DEFAULT, deviceType, MainWindowHandle, vp, &dParam, device);
		if (FAILED(hr)){
			d3d9->Release();
			MessageBox(0, "Create Device Failed.", "Error", MB_OK);
			return false;
		}
	}
	d3d9->Release();

	return true;
}
/*
int Run(bool(*display)(float deltaTime)){
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
			float timeDelta = (currTime - lastTime)*0.001f;

			display(timeDelta);

			lastTime = currTime;
			
		}
	}
	return msg.wParam;
}*/
int Run(bool(*display)(float timeDelta)){
	MSG msg;
	::ZeroMemory(&msg,sizeof(MSG));
	static float lastTime = (float)timeGetTime();
	while (msg.message != WM_QUIT){
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)){
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		else{
			float currTime = (float)timeGetTime();
			float timeDelta = (currTime - lastTime)*0.001f;

			display(timeDelta);

			lastTime = currTime;
		}
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc
(HWND hWnd,
UINT msg,
WPARAM wParam,
LPARAM lParam)
{
	switch (msg){
	case WM_LBUTTONDOWN:
		::MessageBox(0, "Hello,Windows.", "Hello", MB_OK);
		return 0;
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