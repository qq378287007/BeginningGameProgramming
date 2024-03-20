// g++ 03.Direct3DWindowed.cpp -o 03.Direct3DWindowed -mwindows -ld3d9

#ifdef UNICODE
#undef UNICODE
#endif

#include <windows.h>
#include <d3d9.h>
#include <time.h>

// #pragma comment(lib, "d3d9.lib")

// program settings
const char APPTITLE[] = "Direct3D_Windowed";
const int SCREENW = 1024;
const int SCREENH = 768;

// Direct3D objects
LPDIRECT3D9 d3d = NULL;
LPDIRECT3DDEVICE9 d3ddev = NULL; // 视频卡

int gameover = 0;

// macro to detect key presses
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

// Game initialization function
int Game_Init(HWND hwnd)
{
    // initialize Direct3D
    d3d = Direct3DCreate9(D3D_SDK_VERSION);
    if (d3d == NULL)
    {
        MessageBox(hwnd, "Error initializing Direct3D", "Error", MB_OK);
        return FALSE;
    }

    // set Direct3D presentation parameters
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.Windowed = TRUE;                    // TRUE为窗口模式
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; // 后台缓冲区的交换模式
    d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8; // 后台缓冲区格式
    d3dpp.BackBufferCount = 1;                // 后台缓冲区数量
    d3dpp.BackBufferWidth = SCREENW;          // 后台缓冲区宽度
    d3dpp.BackBufferHeight = SCREENH;         // 后台缓冲区高度
    d3dpp.hDeviceWindow = hwnd;               // 本设备的父窗口
    // create Direct3D device
    d3d->CreateDevice(D3DADAPTER_DEFAULT,                  // default video card
                      D3DDEVTYPE_HAL,                      // use the hardware device
                      hwnd,                                // window handle
                      D3DCREATE_SOFTWARE_VERTEXPROCESSING, // do not use T&L(for compatibility)
                      &d3dpp,                              // presentation parameters
                      &d3ddev);                            // pointer to the new device
    if (d3ddev == NULL)
    {
        MessageBox(hwnd, "Error creating Direct3D device", "Error", MB_OK);
        return FALSE;
    }

    return 1;
}

// Game update function
void Game_Run(HWND hwnd)
{
    // make sure the Direct3D device is valid
    if (!d3ddev)
        return;

    // clear the backbuffer to bright green
    d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

    // start rendering
    if (d3ddev->BeginScene())
    {
        // do something?

        // stop rendering
        d3ddev->EndScene();

        // copy back buffer to the frame buffer
        d3ddev->Present(NULL, NULL, NULL, NULL);
    }

    // check for escape key (to exit program)
    if (KEY_DOWN(VK_ESCAPE))
        PostMessage(hwnd, WM_DESTROY, 0, 0);
}

// Game shutdown function
void Game_End(HWND hwnd)
{
    if (d3ddev)
    {
        d3ddev->Release();
        d3ddev = NULL;
    }
    if (d3d)
    {
        d3d->Release();
        d3d = NULL;
    }
}

LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_DESTROY:
        gameover = 1;
        break;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc = {sizeof(WNDCLASSEX),
                     CS_HREDRAW | CS_VREDRAW,
                     (WNDPROC)WinProc,
                     0,
                     0,
                     hInstance,
                     NULL,
                     LoadCursor(NULL, IDC_ARROW),
                     (HBRUSH)GetStockObject(WHITE_BRUSH),
                     NULL,
                     APPTITLE,
                     NULL};
    if (!RegisterClassEx(&wc))
        return FALSE;

    HWND hwnd = CreateWindow(APPTITLE, APPTITLE,
                             WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT, CW_USEDEFAULT,
                             SCREENW, SCREENH,
                             (HWND)NULL,
                             (HMENU)NULL, hInstance, (LPVOID)NULL);
    if (hwnd == 0)
        return 0;
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    if (!Game_Init(hwnd))
        return FALSE;
    MSG msg;
    while (!gameover)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        Game_Run(hwnd);
    }
    Game_End(hwnd);

    UnregisterClass(APPTITLE, hInstance);

    return msg.wParam;
}
