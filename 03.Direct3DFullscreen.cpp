// gcc main.cpp -o main -mwindows -ld3d9
#ifdef UNICODE
#undef UNICODE
#endif

#include <windows.h>
#include <time.h>
#include <d3d9.h>

// #pragma comment(lib, "d3d9.lib")

// program settings
const char APPTITLE[] = "Direct3D_Fullscreen";

// Direct3D objects
LPDIRECT3D9 d3d = NULL;
LPDIRECT3DDEVICE9 d3ddev = NULL;

bool gameover = false;

// macro to detect key presses
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

// Game initialization function
bool Game_Init(HWND hwnd)
{
    // initialize Direct3D
    d3d = Direct3DCreate9(D3D_SDK_VERSION);
    if (d3d == NULL)
    {
        MessageBox(hwnd, "Error initializing Direct3D", "Error", MB_OK);
        return FALSE;
    }

    D3DDISPLAYMODE dm;
    d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &dm); // 屏幕尺寸和颜色深度

    // set Direct3D presentation parameters
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.hDeviceWindow = hwnd;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = dm.Format;
    d3dpp.BackBufferCount = 1;
    d3dpp.BackBufferWidth = dm.Width;
    d3dpp.BackBufferHeight = dm.Height;
    d3dpp.Windowed = FALSE; // 全屏模式

    // create Direct3D device
    d3d->CreateDevice(D3DADAPTER_DEFAULT,
                      D3DDEVTYPE_HAL,
                      hwnd,
                      D3DCREATE_HARDWARE_VERTEXPROCESSING,
                      &d3dpp,
                      &d3ddev);
    if (d3ddev == NULL)
    {
        MessageBox(hwnd, "Error creating Direct3D device", "Error", MB_OK);
        return FALSE;
    }

    return TRUE;
}

// Game update function
void Game_Run(HWND hwnd)
{
    // make sure the Direct3D device is valid
    if (!d3ddev)
        return;

    // clear the backbuffer to bright blue
    d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

    // start rendering
    if (d3ddev->BeginScene())
    {
        // do something?

        // stop rendering
        d3ddev->EndScene();

        // copy back buffer on the screen
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

// Windows event handling function
LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_DESTROY:
        gameover = true;
        break;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

// Windows entry point function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // set the new window's properties
    WNDCLASSEX wc;
    memset(&wc, 0, sizeof(WNDCLASS));
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpszClassName = "MainWindowClass";
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC)WinProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hIcon = 0;
    wc.lpszMenuName = 0;
    wc.hIconSm = 0;
    wc.hbrBackground = 0;
    if (!RegisterClassEx(&wc))
        return FALSE;

    // create a new window
    HWND hwnd = CreateWindow("MainWindowClass",
                             APPTITLE,
                             WS_EX_TOPMOST | WS_POPUP, // 窗口具有焦点且不含边框和标题栏
                             0,
                             0,
                             640,
                             480,
                             (HWND)NULL,
                             (HMENU)NULL,
                             hInstance,
                             (LPVOID)NULL);
    // was there an error creating the window?
    if (hwnd == 0)
        return 0;

    // display the window
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // initialize the game
    if (!Game_Init(hwnd))
        return 0;

    MSG msg;
    // main message loop
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

    return msg.wParam;
}
