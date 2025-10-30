#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <wrl/client.h> 
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include "Application12X.h"

Application12X app;

void KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
{

}


LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        return 0;
    }
    break;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) 
{

        const int width = 800;
        const int height = 800;

        // Register a simple window class
        WNDCLASS wc = {};
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = hInstance;
        wc.lpszClassName = L"DirectX12Triangle";
        RegisterClass(&wc);

        // Create a window
        HWND hwnd = CreateWindow(wc.lpszClassName, L"DirectX 12 Triangle", WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, width, height, nullptr, nullptr, hInstance, nullptr);
        ShowWindow(hwnd, nCmdShow);

        //The device is like a virtual representation of the GPU
        ID3D12Device* device = nullptr;
        HRESULT hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_2, IID_PPV_ARGS(&device));




    /* Initialize the library */
    //if (!glfwInit())
    //    return -1;

    //glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    //glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    /* Create a windowed mode window and its OpenGL context */
   /* app.window = glfwCreateWindow(app.Width, app.Height, "Hello D3D11", nullptr, nullptr);
    if (!app.window)
    {
        glfwTerminate();
        return -1;
    }*/

    /* Make the window's context current */
   /* glfwMakeContextCurrent(app.window);*/



    while (!glfwWindowShouldClose(app.window))
    {
        /* Render here */

        /* Swap front and back buffers */
        //glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
 /*       app.update();

        app.draw();*/

        /*Swap front and back buffers*/
        glfwSwapBuffers(app.window);
    }

    glfwTerminate();
    return 0;

}