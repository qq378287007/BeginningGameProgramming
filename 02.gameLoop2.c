// gcc gameLoop2.c -o gameLoop2 -mwindows

#include <windows.h>
#include <time.h>

const char APPTITLE[] = "Game Loop";
HWND window;
HDC device;
int gameover = 0;

const char *filename = "02.c.bmp";
BITMAP bm;
HDC hdcImage;

// Loads and draws a bitmap from a file and then frees the memory
// (not really suitable for a game loop but it's self contained)
void DrawBitmap(int x, int y)
{
    // draw the bitmap to the window (bit block transfer)
    BitBlt(device,                  // destination device context
           x, y,                    // x,y position
           bm.bmWidth, bm.bmHeight, // size of source bitmap
           hdcImage,                // source device context
           0, 0,                    // upper-left source position
           SRCCOPY);                // blit method
}

// Startup and loading code goes here
int Game_Init()
{
    // get device context for drawing
    device = GetDC(window);

    // start up the random number generator
    srand(time(NULL));

    // load the bitmap image
    HBITMAP image = (HBITMAP)LoadImage(0, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    // read the bitmap's properties
    GetObject(image, sizeof(BITMAP), &bm);

    // create a device context for the bitmap
    hdcImage = CreateCompatibleDC(device);//创建与指定设备兼容的内存设备上下文 (DC)，copy
    SelectObject(hdcImage, image);//对象载入DC中，对象操作自动修改CD内存？

    // delete bitmap
    DeleteObject((HGDIOBJ)image);

    return 1;
}

// Update function called from inside game loop
void Game_Run()
{
    if (gameover == 1)
        return;

    // get the drawing surface
    RECT rect;
    GetClientRect(window, &rect);

    // draw bitmap at random location
    int x = rand() % (rect.right - rect.left);
    int y = rand() % (rect.bottom - rect.top);
    DrawBitmap(x, y);
}

// Shutdown code
void Game_End()
{
    // delete the device context
    DeleteDC(hdcImage);

    // free the device
    ReleaseDC(window, device);
}

// Window callback function
LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        gameover = 1;
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

// Entry point function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc = {
        sizeof(WNDCLASSEX),
        CS_HREDRAW | CS_VREDRAW,
        (WNDPROC)WinProc,
        0,
        0,
        hInstance, // GetModuleHandle(NULL)
        NULL,
        NULL, // LoadCursor(NULL, IDC_ARROW);
        NULL, //(HBRUSH)GetStockObject(WHITE_BRUSH);
        NULL,
        APPTITLE,
        NULL};
    if (RegisterClassEx(&wc) == 0)
    {
        MessageBox(NULL, "RegisterClassEx Fail!", APPTITLE, MB_OK | MB_ICONEXCLAMATION);
        return 0;
    }

    // create a new window
    // previously found in the InitInstance function
    window = CreateWindow(
        APPTITLE,                     // window class
        APPTITLE,                     // title bar
        WS_OVERLAPPEDWINDOW,          // window style
        CW_USEDEFAULT, CW_USEDEFAULT, // position of window
        640, 480,                     // dimensions of the window
        NULL,                         // GetDesktopWindow()  // parent window (not used)
        NULL,                         // menu (not used)
        hInstance,                    // application instance
        NULL);                        // window parameters (not used)
    if (window == 0)
    {
        MessageBox(NULL, "CreateWindow Fail!", APPTITLE, MB_OK | MB_ICONEXCLAMATION);
        return 0;
    }

    ShowWindow(window, nCmdShow);
    UpdateWindow(window);

    if (!Game_Init())
        return 0;

    // main message loop
    MSG msg;
    while (!gameover)
    {
        // process Windows events
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // process game loop
        Game_Run();
    }

    // free game resources
    Game_End();

    UnregisterClass(APPTITLE, hInstance);

    return msg.wParam;
}
