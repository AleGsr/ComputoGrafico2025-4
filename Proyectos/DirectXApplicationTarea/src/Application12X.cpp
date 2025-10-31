#include "Application12X.h"
#include <iostream>

void Application12X::ThrowIfFailedX(HRESULT hr, const std::string& msg)
{

}

void Application12X::ThrowIfFailedX(HRESULT hr)
{

}

void Application12X::setupGeometryX()
{
    std::vector<float> geometry
    {  //  X     Y     Z      W
        -1.0f, -1.0f, 0.0f, 1.0f, //Vectice 2
        -1.0f, 1.0f,  0.0f, 1.0f, //Vectice 1
        1.0f,  -1.0f, 0.0f, 1.0f, //Vectice 3
        1.0f,  1.0f,  0.0f, 1.0f, //vertice 4

        1.0f , 0.0f,  0.0f, 1.0f, //rojo
        0.0f , 1.0f,  0.0f, 1.0f, //verde
        0.0f , 0.0f,  1.0f, 1.0f, //azul
        0.0f , 1.0f,  0.0f, 1.0f, //verde
    };
}

void Application12X::setupShadersX()
{
    //compile shaders
    ID3DBlob* vertex_shader = nullptr; //VSmain, vertex shader
    ID3DBlob* pixel_shader = nullptr; //PSmain, fragment shader
    hr = D3DCompileFromFile(L"shader.hlsl", nullptr, nullptr, "VSMain", "vs_5_0", 0, 0, &vertex_shader, nullptr);
    hr = D3DCompileFromFile(L"shader.hlsl", nullptr, nullptr, "PSMain", "ps_5_0", 0, 0, &pixel_shader, nullptr);

    // Pipeline state
    D3D12_GRAPHICS_PIPELINE_STATE_DESC pso_desc = {};
    pso_desc.pRootSignature = root_signature;
    pso_desc.VS.pShaderBytecode = vertex_shader->GetBufferPointer();
    pso_desc.VS.BytecodeLength = vertex_shader->GetBufferSize();
    pso_desc.PS.pShaderBytecode = pixel_shader->GetBufferPointer();
    pso_desc.PS.BytecodeLength = pixel_shader->GetBufferSize();
    setBlendState(pso_desc.BlendState);
    pso_desc.SampleMask = UINT_MAX;
    setRasterizerState(pso_desc.RasterizerState);
    setDepthStencilState(pso_desc.DepthStencilState);
    pso_desc.InputLayout.pInputElementDescs = nullptr;
    pso_desc.InputLayout.NumElements = 0;
    pso_desc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
    pso_desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    pso_desc.NumRenderTargets = 1;
    pso_desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    pso_desc.SampleDesc.Count = 1;
    pso_desc.SampleDesc.Quality = 0;

    Microsoft::WRL::ComPtr<ID3D12PipelineState> g_pipelineState = nullptr;
    hr = device->CreateGraphicsPipelineState(&pso_desc, IID_PPV_ARGS(&g_pipelineState));

    vertex_shader->Release();
    vertex_shader = nullptr;
    pixel_shader->Release();
    pixel_shader = nullptr;
}

void Application12X::setupDeviceX()
{
    //The device is like a virtual representation of the GPU
    ID3D12Device* device = nullptr;
    HRESULT hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_2, IID_PPV_ARGS(&device));

    
    /* hr = device->CreateCommandQueue(&queue_desc, IID_PPV_ARGS(&command_queue));*/
}

void Application12X::setupCommandQueueX()
{
    //command queue decides which order the command lists should execute. In our case, we only have one command list.
    ID3D12CommandQueue* command_queue = nullptr;
    D3D12_COMMAND_QUEUE_DESC queue_desc = {};
    queue_desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    hr = device->CreateCommandQueue(&queue_desc, IID_PPV_ARGS(&command_queue));

}

void Application12X::setupSwapChainX()
{
    //helper object to create a swap chain
    IDXGIFactory4* factory = nullptr;
    hr = CreateDXGIFactory1(IID_PPV_ARGS(&factory));


    //create swap chain
    DXGI_SWAP_CHAIN_DESC swap_chain_desc = {};
    swap_chain_desc.BufferCount = 2;
    swap_chain_desc.BufferDesc.Width = width;
    swap_chain_desc.BufferDesc.Height = height;
    swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swap_chain_desc.OutputWindow = hwnd;
    swap_chain_desc.SampleDesc.Count = 1;
    swap_chain_desc.Windowed = TRUE;

    IDXGISwapChain* temp_swap_chain = nullptr;
    hr = factory->CreateSwapChain(command_queue, &swap_chain_desc, &temp_swap_chain);

    //cast the swap chain to IDXGISwapChain3 to leverage the latest features
    IDXGISwapChain3* swap_chain = {};
    hr = temp_swap_chain->QueryInterface(IID_PPV_ARGS(&swap_chain));
    temp_swap_chain->Release();
    temp_swap_chain = nullptr;
}

void Application12X::setupDescriptorHeapX()
{
    //memory descriptor heap to store render target views(RTV). Descriptor describes how to interperate resource memory.
    ID3D12DescriptorHeap* rtv_heap = nullptr; //////////////////
    D3D12_DESCRIPTOR_HEAP_DESC rtv_heap_desc = {};
    rtv_heap_desc.NumDescriptors = 2;
    rtv_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    hr = device->CreateDescriptorHeap(&rtv_heap_desc, IID_PPV_ARGS(&rtv_heap));

    ID3D12Resource* render_targets[2];

    UINT rtv_increment_size = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    {
        D3D12_CPU_DESCRIPTOR_HANDLE rtv_handle(rtv_heap->GetCPUDescriptorHandleForHeapStart());
        for (UINT i = 0; i < 2; i++) {
            hr = swap_chain->GetBuffer(i, IID_PPV_ARGS(&render_targets[i]));

            device->CreateRenderTargetView(render_targets[i], nullptr, rtv_handle);
            rtv_handle.ptr += rtv_increment_size;
        }
    }
}

void Application12X::setupRenderTargetViewX()
{
}

void Application12X::setupCommandAllocatorX()
{
    // Record commands to draw a triangle
    hr = command_allocator->Reset();
    hr = command_list->Reset(command_allocator.Get(), nullptr);

    UINT back_buffer_index = swap_chain->GetCurrentBackBufferIndex();

    D3D12_CPU_DESCRIPTOR_HANDLE rtv_handle = rtv_heap->GetCPUDescriptorHandleForHeapStart();
    rtv_handle.ptr += back_buffer_index * rtv_increment_size;
}

void Application12X::setupCommandListX()
{
    //command_list->SetGraphicsRoot32BitConstant(0, triangle_angle, 0);
    command_list->DrawInstanced(3, 1, 0, 0);

    {
        D3D12_RESOURCE_BARRIER barrier = {}; /////////////////////
        barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
        barrier.Transition.pResource = render_targets[back_buffer_index];
        barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
        barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
        barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
        command_list->ResourceBarrier(1, &barrier);
    }
}

void Application12X::swapBuffersX()
{
    hr = swap_chain->Present(1, 0);
}

void Application12X::setupX()
{

}

void Application12X::updateX()
{
}

void Application12X::drawX()
{
   
}

void Application12X::signatureX()
{
    D3D12_ROOT_PARAMETER root_parameters[1] = {};
    root_parameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
    root_parameters[0].Constants.Num32BitValues = 1;
    root_parameters[0].Constants.ShaderRegister = 0;
    root_parameters[0].Constants.RegisterSpace = 0;
    root_parameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;

    ID3D12RootSignature* root_signature = nullptr;
    D3D12_ROOT_SIGNATURE_DESC root_signature_desc = {};
    root_signature_desc.NumParameters = _countof(root_parameters);
    root_signature_desc.pParameters = root_parameters;
    root_signature_desc.NumStaticSamplers = 0;
    root_signature_desc.pStaticSamplers = nullptr;
    root_signature_desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    ID3DBlob* signature_blob = nullptr;
    ID3DBlob* error_blob = nullptr;
    hr = D3D12SerializeRootSignature(&root_signature_desc, D3D_ROOT_SIGNATURE_VERSION_1, &signature_blob, &error_blob);
    hr = device->CreateRootSignature(0, signature_blob->GetBufferPointer(), signature_blob->GetBufferSize(), IID_PPV_ARGS(&root_signature));

    if (signature_blob) {
        signature_blob->Release();
        signature_blob = nullptr;
    }
    if (error_blob) {
        error_blob->Release();
        error_blob = nullptr;
    }
}

void Application12X::keyCallbackX(int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    //teclas para mover	

}

