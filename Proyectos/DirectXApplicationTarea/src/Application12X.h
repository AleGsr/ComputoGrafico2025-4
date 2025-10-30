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
	void ThrowIfFailed(HRESULT hr, const std::string& msg);
	void ThrowIfFailed(HRESULT hr);
	void setupGeometry();
	void setupShaders();
	void setupDevice();
	void setupCommandQueue();
	void setupSwapChain();
	void setupDescriptorHeap();
	void setupRenderTargetView();
	void setupCommandAllocator();
	void setupCommandList();
	void swapBuffers();

	std::string readFile(const std::string& filename);

	// --- Configuración y Constantes ---
	static const UINT BUFFER_COUNT = 2; // Double buffering
	const int WINDOW_WIDTH = 1024;
	const int WINDOW_HEIGHT = 768;

	// --- Variables Globales de DX12 ---
	Microsoft::WRL::ComPtr<IDXGIFactory4> factory;

	Microsoft::WRL::ComPtr<ID3D12Device> g_device;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> g_commandQueue;
	Microsoft::WRL::ComPtr<IDXGISwapChain4> g_swapChain;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> g_rtvHeap;  ////////////////////////
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> g_commandAllocator;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> g_commandList;


	Microsoft::WRL::ComPtr<ID3D12Resource> g_renderTargets[BUFFER_COUNT];
	UINT g_frameIndex;
	UINT g_rtvDescriptorSize;


public:

	const int Width{ 1280 };
	const int Height{ 1024 };
	GLFWwindow* window;


	ID3D12Device* device = nullptr;
	HRESULT hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_2, IID_PPV_ARGS(&device));
	

	////command allocator is used to allocate memory on the GPU for commands
	//Microsoft::WRL::ComPtr<ID3D12CommandAllocator> command_allocator = nullptr;
	//hr = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&command_allocator));
	//hr = command_allocator->Reset();


	void setupDeviceX();

	void setupX();
	void updateX();
	void drawX();

	void signatureX();
	void clearColorBuffer(const float& r, const float& g, const float& b, const float& a);
	void keyCallbackX(int key, int scancode, int action, int mods);


};