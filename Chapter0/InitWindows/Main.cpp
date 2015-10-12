#include "InitWin.h"

int WINAPI WinMain(HINSTANCE hInstance,
				HINSTANCE hPrevInstance,
				PSTR pCmdLine,
				int nShowCmd){
	if (!InitWindowsApp(hInstance, nShowCmd)){
		::MessageBox(0, "Init Windows Failed.", "Error", MB_OK);
		return 0;
	}

	return Run();
}