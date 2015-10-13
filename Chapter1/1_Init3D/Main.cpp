#include "InitDX3D.h"

IDirect3DDevice9 * device = 0;

bool Setup(){ 
	return true; 
}
void Cleanup(){
}

bool Display(float timeDelta){
	if (device){
		//将后台缓存和深度设置为黑色和1.0
		device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
		//提交后台缓存
		device->Present(0, 0, 0, 0);
	}
	return true;
}

int WINAPI WinMain
   (HINSTANCE hInstance,
	HINSTANCE hPreInstance,
	PSTR lpStr,
	int nShowCmd)
{
	
	if (!InitWindowsApp(hInstance, nShowCmd,800, 600)){
		::MessageBox(0,"Init Window Failed.","Error",MB_OK);
		return 0;
	}
	
	if (!InitDirect3D(D3DDEVTYPE_HAL, &device,800,600)){
		::MessageBox(0,"Init 3D Failed.","Error",MB_OK);
		return 0;
	}

	if (!Setup()){
		::MessageBox(0, "Setup Failed.", "Error", MB_OK);
		return 0;
	}

	Run(Display);

	Cleanup();

	device->Release();

	return 0;
}