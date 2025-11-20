#include "Application.h"
#include <iostream>
#include <fstream>
#include <sstream>

void Application::ThrowIfFailed(HRESULT hr, const std::string& msg) {
	if (FAILED(hr)) {
		throw std::runtime_error(msg);
	}
}

void Application::ThrowIfFailed(HRESULT hr) {
	ThrowIfFailed(hr, ":(");
}

void Application::setBlendState(D3D12_BLEND_DESC& blend_desc) {
	blend_desc = {};

	blend_desc.AlphaToCoverageEnable = FALSE;
	blend_desc.IndependentBlendEnable = FALSE;

	D3D12_RENDER_TARGET_BLEND_DESC default_render_target_blend_desc = {};
	default_render_target_blend_desc.BlendEnable = FALSE;
	default_render_target_blend_desc.LogicOpEnable = FALSE;
	default_render_target_blend_desc.SrcBlend = D3D12_BLEND_ONE;
	default_render_target_blend_desc.DestBlend = D3D12_BLEND_ZERO;
	default_render_target_blend_desc.BlendOp = D3D12_BLEND_OP_ADD;
	default_render_target_blend_desc.SrcBlendAlpha = D3D12_BLEND_ONE;
	default_render_target_blend_desc.DestBlendAlpha = D3D12_BLEND_ZERO;
	default_render_target_blend_desc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	default_render_target_blend_desc.LogicOp = D3D12_LOGIC_OP_NOOP;
	default_render_target_blend_desc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++) {
		blend_desc.RenderTarget[i] = default_render_target_blend_desc;
	}
}

void Application::setRasterizerState(D3D12_RASTERIZER_DESC& rasterizer_desc) {
	rasterizer_desc = {};

	rasterizer_desc.FillMode = D3D12_FILL_MODE_SOLID;
	rasterizer_desc.CullMode = D3D12_CULL_MODE_BACK;
	rasterizer_desc.FrontCounterClockwise = FALSE;
	rasterizer_desc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
	rasterizer_desc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
	rasterizer_desc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
	rasterizer_desc.DepthClipEnable = TRUE;
	rasterizer_desc.MultisampleEnable = FALSE;
	rasterizer_desc.AntialiasedLineEnable = FALSE;
	rasterizer_desc.ForcedSampleCount = 0;
	rasterizer_desc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
}

void Application::setDepthStencilState(D3D12_DEPTH_STENCIL_DESC& depth_stencil_desc) {
	depth_stencil_desc = {};

	depth_stencil_desc.DepthEnable = TRUE;
	depth_stencil_desc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	depth_stencil_desc.DepthFunc = D3D12_COMPARISON_FUNC_GREATER;

	depth_stencil_desc.StencilEnable = FALSE;
	depth_stencil_desc.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
	depth_stencil_desc.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;

	depth_stencil_desc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	depth_stencil_desc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	depth_stencil_desc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	depth_stencil_desc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;

	depth_stencil_desc.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	depth_stencil_desc.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	depth_stencil_desc.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	depth_stencil_desc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
}

std::string Application::readFile(const std::string& filename)
{
	std::ifstream myFile(filename);

	if (!myFile.is_open()) {
		return "";
	}

	std::stringstream buffer;
	buffer << myFile.rdbuf();

	return buffer.str();
}

HWND Application::GetWindowNativeHandler() const
{
	return glfwGetWin32Window(window);
}

void Application::setupGeometry()
{
	Model model = load_model_from_obj("rabbit.obj");
	//heap properties
	D3D12_HEAP_PROPERTIES heap_properties = {};
	heap_properties.Type = D3D12_HEAP_TYPE_DEFAULT;
	heap_properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heap_properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heap_properties.CreationNodeMask = 1;
	heap_properties.VisibleNodeMask = 1;

	D3D12_HEAP_PROPERTIES heap_properties_upload = heap_properties;
	heap_properties_upload.Type = D3D12_HEAP_TYPE_UPLOAD;

	//resource description
	D3D12_RESOURCE_DESC resource_desc = {};
	resource_desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resource_desc.Alignment = 0;
	resource_desc.Width = sizeof(Vertex) * model.vertices.size();
	resource_desc.Height = 1;
	resource_desc.DepthOrArraySize = 1;
	resource_desc.MipLevels = 1;
	resource_desc.Format = DXGI_FORMAT_UNKNOWN;
	resource_desc.SampleDesc.Count = 1;
	resource_desc.SampleDesc.Quality = 0;
	resource_desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resource_desc.Flags = D3D12_RESOURCE_FLAG_NONE;

	//vertex
	device->CreateCommittedResource(
		&heap_properties,
		D3D12_HEAP_FLAG_NONE,
		&resource_desc,
		D3D12_RESOURCE_STATE_COMMON,
		nullptr,
		IID_PPV_ARGS(&vertex_buffer) //.Get
	);

	device->CreateCommittedResource(
		&heap_properties_upload,
		D3D12_HEAP_FLAG_NONE,
		&resource_desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertex_buffer_upload)
	);

	//index
	resource_desc.Width = sizeof(unsigned int) * model.indicies.size();
	device->CreateCommittedResource(
		&heap_properties,
		D3D12_HEAP_FLAG_NONE,
		&resource_desc,
		D3D12_RESOURCE_STATE_COMMON,
		nullptr,
		IID_PPV_ARGS(&index_buffer)
	);

	device->CreateCommittedResource(
		&heap_properties_upload,
		D3D12_HEAP_FLAG_NONE,
		&resource_desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&index_buffer_upload)
	);
	////////////////////////////// Checar los bloques de arriba
	//copy data from CPU to the upload buffers
	void* vertex_mapped_data = nullptr;
	vertex_buffer_upload->Map(0, nullptr, &vertex_mapped_data);
	memcpy(vertex_mapped_data, model.vertices.data(), sizeof(Vertex) * model.vertices.size());
	vertex_buffer_upload->Unmap(0, nullptr);

	void* index_mapped_data = nullptr;
	index_buffer_upload->Map(0, nullptr, &index_mapped_data);
	memcpy(index_mapped_data, model.indicies.data(), sizeof(unsigned int) * model.indicies.size());
	index_buffer_upload->Unmap(0, nullptr);

	//Record commands to copy the data from the upload buffer to the fast default buffer
	commandAllocator->Reset();
	commandList->Reset(commandAllocator.Get(), nullptr); //Ultima linea

	D3D12_RESOURCE_BARRIER barrier[2] = {}; //Barrier sincroniza 
	barrier[0].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier[0].Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier[0].Transition.pResource = vertex_buffer.Get();
	barrier[0].Transition.StateBefore = D3D12_RESOURCE_STATE_COMMON;
	barrier[0].Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier[0].Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	barrier[1].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier[1].Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier[1].Transition.pResource = index_buffer.Get();
	barrier[1].Transition.StateBefore = D3D12_RESOURCE_STATE_COMMON;
	barrier[1].Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier[1].Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	commandList->ResourceBarrier(2, barrier);

	//copy the data from upload to the fast default buffer
	commandList->CopyResource(vertex_buffer.Get(), vertex_buffer_upload.Get());
	commandList->CopyResource(index_buffer.Get(), index_buffer_upload.Get());

	barrier[0].Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier[0].Transition.StateAfter = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;

	barrier[1].Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier[1].Transition.StateAfter = D3D12_RESOURCE_STATE_INDEX_BUFFER;

	commandList->ResourceBarrier(2, barrier);

	commandList->Close();

	ID3D12CommandList* command_lists[] = { commandList.Get()};
	commandQueue->ExecuteCommandLists(1, command_lists);

	// Wait on the CPU for the GPU frame to finish
	const UINT64 current_fence_value = ++fence_value;
	commandQueue->Signal(fence, current_fence_value);

	if (fence->GetCompletedValue() < current_fence_value) {
		fence->SetEventOnCompletion(current_fence_value, fence_event);
		WaitForSingleObject(fence_event, INFINITE);
	}
}


void Application::keyCallback(int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	//teclas para mover	
}

void Application::setupShaders()
{
	//compile shaders
	ID3DBlob* vertex_shader = nullptr;
	ID3DBlob* pixel_shader = nullptr;
	ThrowIfFailed(D3DCompileFromFile(L"Shaders/shader.hlsl", nullptr, nullptr, "VSMain", "vs_5_0", 0, 0, &vertex_shader, nullptr), "Error compiling shader 1");
	ThrowIfFailed(D3DCompileFromFile(L"Shaders/shader.hlsl", nullptr, nullptr, "PSMain", "ps_5_0", 0, 0, &pixel_shader, nullptr), "Error compiling shader 2");

	// Pipeline state
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pso_desc = {};
	pso_desc.pRootSignature = rootSignature.Get();
	pso_desc.VS.pShaderBytecode = vertex_shader->GetBufferPointer();
	pso_desc.VS.BytecodeLength = vertex_shader->GetBufferSize();
	pso_desc.PS.pShaderBytecode = pixel_shader->GetBufferPointer();
	pso_desc.PS.BytecodeLength = pixel_shader->GetBufferSize();
	setBlendState(pso_desc.BlendState);
	pso_desc.SampleMask = UINT_MAX;
	setRasterizerState(pso_desc.RasterizerState);
	setDepthStencilState(pso_desc.DepthStencilState);

	//-Agregado
	D3D12_INPUT_ELEMENT_DESC input_elements[] = {
		   {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		   {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	};
	//Agregado-

	//pso_desc.InputLayout.pInputElementDescs = nullptr;
	pso_desc.InputLayout.pInputElementDescs = input_elements;
	//pso_desc.InputLayout.NumElements = 0;
	pso_desc.InputLayout.NumElements = _countof(input_elements);
	pso_desc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
	pso_desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	pso_desc.NumRenderTargets = 1;
	pso_desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	pso_desc.DSVFormat = DXGI_FORMAT_D32_FLOAT; //Linea extra
	pso_desc.SampleDesc.Count = 1;
	pso_desc.SampleDesc.Quality = 0;

	pipelineState = nullptr;
	ThrowIfFailed(device->CreateGraphicsPipelineState(&pso_desc, IID_PPV_ARGS(&pipelineState)), "Error creating pipeline)");

	vertex_shader->Release();
	vertex_shader = nullptr;
	pixel_shader->Release();
	pixel_shader = nullptr;


}

void Application::setupFenceEvent()
{
	device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
}


void Application::setupConstantBuffer() //YA NO SE MUEVE NADA
{
	//Crear un buffer en la GPU con tipo UPLOAD (CPU-write, GPU-read
	D3D12_HEAP_PROPERTIES heapProps = {};
	heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProps.CreationNodeMask = 1;
	heapProps.VisibleNodeMask = 1;

	D3D12_RESOURCE_DESC resourceDesc = {};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	//resourceDesc.Width = (sizeof(SceneConstants) + 255) & 255; //Multiplo de 256 bytes (requerido)
	resourceDesc.Width = sizeof(SceneConstants);
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	device->CreateCommittedResource(
		&heapProps,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constantBuffer)
	);

	ThrowIfFailed(constantBuffer->Map(0, nullptr, &mappedMemory), "fallo al copiar constantes");


}



void Application::setupDevice() //Modificado
{
	//// Crear el Device (Buscando el adaptador de hardware)
	Microsoft::WRL::ComPtr<IDXGIAdapter1> adapter;
	for (UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != factory->EnumAdapters1(adapterIndex, &adapter); ++adapterIndex) {
		DXGI_ADAPTER_DESC1 desc;
		adapter->GetDesc1(&desc);
		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) continue;
		if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_12_2, IID_PPV_ARGS(&device)))) {
			break;
		}
	}

	//Agregado
	HRESULT hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device));
}

void Application::setupCommandQueue() //No modificado pero si en DXModel
{
	//command queue decides which order the command lists should execute. In our case, we only have one command list.	
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	ThrowIfFailed(device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&commandQueue)), "Failed Command Queue");
}

void Application::setupSwapChain()
{
	//create swap chain
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferCount = 2;
	swapChainDesc.BufferDesc.Width = WINDOW_WIDTH;
	swapChainDesc.BufferDesc.Height = WINDOW_HEIGHT;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.OutputWindow = GetWindowNativeHandler(); //Es diferente al DXModel
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.Windowed = TRUE;

	IDXGISwapChain* tempSwapChain = nullptr;
	ThrowIfFailed(factory->CreateSwapChain(commandQueue.Get(), &swapChainDesc, &tempSwapChain), "Failed to create swapchain");

	//cast the swap chain to IDXGISwapChain4 to leverage the latest features
	ThrowIfFailed(tempSwapChain->QueryInterface(IID_PPV_ARGS(&swapChain)), "Error casting to swap chain");
	tempSwapChain->Release();
	tempSwapChain = nullptr;

	//Hasta linea 229 de DXModel
}

void Application::setupDescriptorHeap() //No se modifica
{
	//memory descriptor heap to store render target views(RTV). Descriptor describes how to interperate resource memory.
	rtvHeap = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
	rtvHeapDesc.NumDescriptors = 2;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	ThrowIfFailed(device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeap)), "Error creating heap");
}

void Application::setupRenderTargetView()
{
	//Crear Render Target Views(RTVs)	
	rtvIncrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	{
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle(rtvHeap->GetCPUDescriptorHandleForHeapStart());
		for (UINT i = 0; i < 2; i++) {
			ThrowIfFailed(swapChain->GetBuffer(i, IID_PPV_ARGS(&renderTargets[i])), "Failed to create RTV");

			device->CreateRenderTargetView(renderTargets[i].Get(), nullptr, rtvHandle);
			rtvHandle.ptr += rtvIncrementSize;
		}
	}
}


void Application::setupSignature()
{
	//Root signature is like have many object buffers and textures we want to use when drawing.
	//For our rotating triangle, we only need a single constant that is going to be our angle
	D3D12_ROOT_PARAMETER rootParameters[1] = {};
	//rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;;
	rootParameters[0].Constants.Num32BitValues = 1; //Linea extra
	rootParameters[0].Constants.ShaderRegister = 0; //register(b0)
	rootParameters[0].Constants.RegisterSpace = 0;
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;

	rootSignature = nullptr;
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
	rootSignatureDesc.NumParameters = _countof(rootParameters);
	rootSignatureDesc.pParameters = rootParameters;
	rootSignatureDesc.NumStaticSamplers = 0;
	rootSignatureDesc.pStaticSamplers = nullptr;
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	ID3DBlob* signatureBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	ThrowIfFailed(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob), "Error creating signature 1");
	ThrowIfFailed(device->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature)), "Error creating signature 2");

	if (signatureBlob) {
		signatureBlob->Release();
		signatureBlob = nullptr;
	}
	if (errorBlob) {
		errorBlob->Release();
		errorBlob = nullptr;
	}
}

void Application::setupCommandAllocator() //DXModel linea 197
{
	//Crear Command Allocator y Command List
	ThrowIfFailed(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator)),
		"Error creando Command Allocator");
	ThrowIfFailed(commandAllocator->Reset(), "Error reseting command Allocator");
}

void Application::setupCommandList()  //DXModel linea 202
{
	//command list is used to store a list of commands that we want to execute on the GPU
	ThrowIfFailed(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator.Get(), nullptr, IID_PPV_ARGS(&commandList)),
		"Failed to create Command list");
	ThrowIfFailed(commandList->Close(), "Failed to close Commandlist");
}



void Application::update() //Van sin el transpose
{
	++triangle_angle;
	sceneConstants.eye = DirectX::XMVectorSet(0.0f, 0.0f, -3.0f, 1.0f); //Posición de la cámara
	sceneConstants.center = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f); //Punto al que mira
	sceneConstants.up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); //Vector 'Up' la w va en 0 porque no es un vector

	sceneConstants.view = DirectX::XMMatrixTranspose(DirectX::XMMatrixLookAtLH(sceneConstants.eye, sceneConstants.center, sceneConstants.up));

	//sceneConstants.triangleAngle = DirectX::XMMatrixRotationAxis(DirectX::XMVECTOR Axis, float Angle);

	float aspect = static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT);
	sceneConstants.projection = DirectX::XMMatrixIdentity();
	DirectX::XMVECTOR rotationAxis = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f);
	sceneConstants.model = DirectX::XMMatrixTranspose(DirectX::XMMatrixScaling(0.5f, 0.5f, 0.5f));

}

void Application::draw()
{
	// Record commands to draw a triangle
	UINT h = commandAllocator->Reset();
	h = commandList->Reset(commandAllocator.Get(), nullptr);

	UINT back_buffer_index = swapChain->GetCurrentBackBufferIndex();

	{
		D3D12_RESOURCE_BARRIER barrier = {};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.pResource = renderTargets[back_buffer_index].Get();
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		commandList->ResourceBarrier(1, &barrier);
	}

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
	rtvHandle.ptr += back_buffer_index * rtvIncrementSize; //Indice y posicion de inicio

	// Clear the render target
	float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
	commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

	//AÑADIDO
	D3D12_CPU_DESCRIPTOR_HANDLE dsv_handle = dsv_heap->GetCPUDescriptorHandleForHeapStart();
	commandList->ClearDepthStencilView(dsv_handle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	// Set viewport and scissor
	D3D12_VIEWPORT viewport = { 0.0f, 0.0f, static_cast<float>(WINDOW_WIDTH), static_cast<float>(WINDOW_HEIGHT), 0.0f, 1.0f };
	D3D12_RECT scissor_rect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
	commandList->RSSetViewports(1, &viewport);
	commandList->RSSetScissorRects(1, &scissor_rect);
	commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	commandList->SetGraphicsRootSignature(rootSignature.Get());
	commandList->SetPipelineState(pipelineState.Get());  //Shaders, selecciona los shaders que se van a usar


	////Copiar los datos de la estructura al constant bugffer  //POSIBLE LINEA 451-464
	//memcpy(mappedMemory, &sceneConstants, sizeof(SceneConstants));

	//commandList->SetGraphicsRootConstantBufferView(0, constantBuffer->GetGPUVirtualAddress());
	/////---------------


	D3D12_VERTEX_BUFFER_VIEW vertex_buffer_view = {};
	vertex_buffer_view.BufferLocation = vertex_buffer->GetGPUVirtualAddress();
	vertex_buffer_view.StrideInBytes = sizeof(Vertex);
	vertex_buffer_view.SizeInBytes = sizeof(Vertex) * model.vertices.size();
	commandList->IASetVertexBuffers(0, 1, &vertex_buffer_view);

	D3D12_INDEX_BUFFER_VIEW index_buffer_view = {};
	index_buffer_view.BufferLocation = index_buffer->GetGPUVirtualAddress();
	index_buffer_view.SizeInBytes = sizeof(unsigned int) * load_model_from_obj("rabbit.obj").indicies.size();
	index_buffer_view.Format = DXGI_FORMAT_R32_UINT;
	commandList->IASetIndexBuffer(&index_buffer_view);


	commandList->DrawIndexedInstanced(load_model_from_obj("rabbit.obj").indicies.size(), 1, 0, 0, 0);
	{
		D3D12_RESOURCE_BARRIER barrier = {};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.pResource = renderTargets[back_buffer_index].Get();
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		commandList->ResourceBarrier(1, &barrier);
	}

	h = commandList->Close();

	ID3D12CommandList* commandLists[] = { commandList.Get() };
	commandQueue->ExecuteCommandLists(1, commandLists);

	h = swapChain->Present(1, 0); //NO está agregado en DXModel

}

void Application::setup()
{
	//Inicializa DirectX 12
	// Crear el DXGI Factory		
	ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&factory)), "Error creating Factory");
	setupDevice();
	setupGeometry();

	setupCommandQueue();
	setupCommandAllocator();
	setupCommandList();
	setupSwapChain();
	setupDescriptorHeap();
	setupRenderTargetView();
	setupSignature();
	setupShaders();
	setupConstantBuffer();
}

Model Application:: load_model_from_obj(const std::string& path) {
	std::ifstream file(path);  // Open the file

	if (!file.is_open()) {
		return {};
	}


	Model model;
	std::vector<DirectX::XMFLOAT3> temp_positions;
	std::vector<DirectX::XMFLOAT3> temp_normals;
	std::map<std::pair<unsigned int, unsigned int>, unsigned int> index_map;
	std::vector<unsigned int> temp_face_indices;

	std::string line;
	while (std::getline(file, line)) {
		std::stringstream ss(line);

		std::string prefix;
		ss >> prefix;

		if (prefix == "v") { //vertex position
			DirectX::XMFLOAT3 position;
			ss >> position.x >> position.y >> position.z;
			temp_positions.push_back(position);
		}
		else if (prefix == "vn") { //vertex normal
			DirectX::XMFLOAT3 normal;
			ss >> normal.x >> normal.y >> normal.z;
			temp_normals.push_back(normal);
		}
		else if (prefix == "f") {
			temp_face_indices.clear();

			std::string vertexindex_slashslash_normalindex;
			while (ss >> vertexindex_slashslash_normalindex) {
				size_t slash_pos = vertexindex_slashslash_normalindex.find("//");

				std::string vertex_index_str = vertexindex_slashslash_normalindex.substr(0, slash_pos);
				std::string normal_index_str = vertexindex_slashslash_normalindex.substr(slash_pos + 2);
				unsigned int vertex_index = std::stoi(vertex_index_str) - 1;
				unsigned int normal_index = std::stoi(normal_index_str) - 1;

				// Check if the vertex and normal index pair already exists in the map
				auto it = index_map.find({ vertex_index, normal_index });

				if (it != index_map.end()) {
					temp_face_indices.push_back(it->second);
				}
				else {
					// If it doesn't exist, create a new vertex and add it to the model
					Vertex vertex;
					vertex.position = temp_positions[vertex_index];
					vertex.normal = temp_normals[normal_index];
					unsigned int new_index = static_cast<unsigned int>(model.vertices.size());
					model.vertices.push_back(vertex);
					// Store the new index in the map
					index_map[{ vertex_index, normal_index }] = new_index;
					temp_face_indices.push_back(new_index);
				}
			}

			for (int i = 0; i < (int)temp_face_indices.size() - 2; i++) {
				model.indicies.push_back(temp_face_indices[0]);
				model.indicies.push_back(temp_face_indices[i + 1]);
				model.indicies.push_back(temp_face_indices[i + 2]);
			}
		}
	}
	file.close();  // Close the file
	return model;
}

