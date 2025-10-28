#pragma once

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
// Para obtener el HWND de la ventana de GLFW
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"

#include <iostream>
#include <vector>
#include <windows.h>


// Librerías de DirectX 12
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl/client.h> // Para Microsoft::WRL::ComPtr


#include <map>
#include <string>


class Application12X
{

private:



public:

	const int Width{ 1280 };
	const int Height{ 1024 };
	GLFWwindow* window;


	ID3D12Device* device = nullptr;
	HRESULT hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_2, IID_PPV_ARGS(&device));
	

	//command allocator is used to allocate memory on the GPU for commands
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> command_allocator = nullptr;
	hr = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&command_allocator));
	hr = command_allocator->Reset();


	void setupDeviceX();

	void setupX();
	void updateX();
	void drawX();

	void signatureX();

	void keyCallbackX(int key, int scancode, int action, int mods);


};