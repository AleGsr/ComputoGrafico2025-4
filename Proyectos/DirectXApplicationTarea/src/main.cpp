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
    app.keyCallback(key, scancode, action, mods);
}

int main(void)
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    app.window = glfwCreateWindow(1024, 768, "Hello Application", NULL, NULL);
    if (!app.window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(app.window);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        //Manejar error
        return -1;
    }

    app.setup();

    glfwSetKeyCallback(app.window, KeyCallBack);

    while (!glfwWindowShouldClose(app.window))
    {
        glfwPollEvents();
        app.update();

        app.draw();
    }

    glfwTerminate();
    return 0;
}