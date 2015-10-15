#include"InitWin.h"

const int width = 800;
const int height = 600;

IDirect3DDevice9 *device = 0;
IDirect3DVertexBuffer9 * vb = 0;
IDirect3DIndexBuffer9 * ib = 0;

bool Setup(){
	//Create Vertex Buffer
	device->CreateVertexBuffer(8 * sizeof(Vertex), D3DUSAGE_WRITEONLY, Vertex::FVF, D3DPOOL_MANAGED, &vb, 0);
	//create Index Buffer
	device->CreateIndexBuffer(36 * sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &ib, 0);
	//fill the buffers with cube data.
	/*
	Lock中的第三个参数是指向被锁定的存储区起始位置的指针，即指针的指针
	*/
	Vertex* vertices;
	vb->Lock(0, 0, (void**)&vertices, 0);
	vertices[0] = Vertex(-1.0f, -1.0f, -1.0f);
	vertices[1] = Vertex(-1.0f, 1.0f, -1.0f);
	vertices[2] = Vertex(1.0f, 1.0f, -1.0f);
	vertices[3] = Vertex(1.0f, -1.0f, -1.0f);
	vertices[4] = Vertex(-1.0f, -1.0f, 1.0f);
	vertices[5] = Vertex(-1.0f, 1.0f, 1.0f);
	vertices[6] = Vertex(1.0f, 1.0f, 1.0f);
	vertices[7] = Vertex(1.0f, -1.0f, 1.0f);
	vb->Unlock();

	//define the triangles of the cube
	WORD * indices = 0;
	ib->Lock(0, 0, (void**)&indices, 0);
	//front side
	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 0; indices[4] = 2; indices[5] = 3;
	//back side
	indices[6] = 4; indices[7] = 6; indices[8] = 5;
	indices[9] = 4; indices[10] = 7; indices[11] = 6;
	//left side  (differ)
	indices[12] = 0; indices[13] = 5; indices[14] = 1;
	indices[15] = 0; indices[16] = 4; indices[17] = 5;
	//right side
	indices[18] = 3; indices[19] = 2; indices[20] = 6;
	indices[21] = 3; indices[22] = 6; indices[23] = 7;
	//top side
	indices[24] = 1; indices[25] = 5; indices[26] = 6;
	indices[27] = 1; indices[28] = 6; indices[29] = 2;
	//bottom side
	indices[30] = 4; indices[31] = 0; indices[32] = 3;
	indices[33] = 4; indices[34] = 3; indices[35] = 7;
	ib->Unlock();

	//camera's position and aim
	D3DXVECTOR3 position(0.0f,0.0f,-5.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMATRIX v;
	D3DXMatrixLookAtLH(&v, &position, &target, &up);

	device->SetTransform(D3DTS_VIEW, &v);
	//projection matrix  
	D3DXMATRIX proj;
	//second parameter:观察范围夹角
	D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI*0.5f, (float)width / (float)height, 1.0f, 1000.0f);
	device->SetTransform(D3DTS_PROJECTION, &proj);
	//绘制状态：采用线框模式。
	device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	return true;
}

void Cleanup(){
	d3d9::Release<IDirect3DVertexBuffer9*>(vb);
	d3d9::Release<IDirect3DIndexBuffer9*>(ib);
}

bool Display(float timeDelta){
	if (device){
		D3DXMATRIX rx, ry;
		//rotate 45 degrees on x-axis
		D3DXMatrixRotationX(&rx, 3.14f / 4.0f);
		//increment y-rotation angle each frame
		static float y = 0.0f;
		D3DXMatrixRotationY(&ry, y);
		y += timeDelta;
		if (y >= 6.28f)
			y = 0.0f;
		D3DXMATRIX p = rx*ry;
		device->SetTransform(D3DTS_WORLD, &p);

		device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
		device->BeginScene();
		
		device->SetStreamSource(0, vb, 0, sizeof(Vertex));
		device->SetIndices(ib);
		device->SetFVF(Vertex::FVF);
		device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);

		device->EndScene();
		device->Present(0, 0, 0, 0);
	}
	return true;
}

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPreInstance,
	PSTR lpStr,
	int nShowCmd)
{
	if (!d3d9::InitWindowsApp(hInstance, nShowCmd, width, height)){
		::MessageBox(0, "Init Windows Failed.", "Error", MB_OK);
		return 0;
	}
	if (!d3d9::InitDX3D(D3DDEVTYPE_HAL, &device, width, height)){
		::MessageBox(0, "Init DX3D Failed.", "Error", MB_OK);
		return 0;
	}

	if (!Setup()){
		::MessageBox(0, "Setup Resources Failed.", "Error", MB_OK);
		return 0;
	}
	d3d9::Run(Display);

	Cleanup();

	device->Release();

	return 0;
}