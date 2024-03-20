// gcc gameLoop1.c -o gameLoop1 -mwindows

#include <windows.h>
#include <time.h>

const char APPTITLE[] = "Game Loop";
HWND window;
HDC device;
int gameover = 0;

// Loads and draws a bitmap from a file and then frees the memory
// (not really suitable for a game loop but it's self contained)
void DrawBitmap(const char *filename, int x, int y)
{
    // load the bitmap image
    HBITMAP image = (HBITMAP)LoadImage(0, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    // create a device context for the bitmap
    HDC hdcImage = CreateCompatibleDC(device);
    SelectObject(hdcImage, image);

    // read the bitmap's properties
    BITMAP bm;
    GetObject(image, sizeof(BITMAP), &bm);

    // draw the bitmap to the window (bit block transfer)
    BitBlt(
        device,                  // destination device context
        x, y,                    // x,y position
        bm.bmWidth, bm.bmHeight, // size of source bitmap
        hdcImage,                // source device context
        0, 0,                    // upper-left source position
        SRCCOPY);                // blit method

    // delete the device context and bitmap
    DeleteDC(hdcImage);
    DeleteObject((HBITMAP)image);
}

// Startup and loading code goes here
int Game_Init()
{
    // get device context for drawing
    device = GetDC(window);

    // start up the random number generator
    srand(time(NULL));

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
    DrawBitmap("02.c.bmp", x, y);
}

// Shutdown code
void Game_End()
{
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

// MyRegiserClass function sets program window properties
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    // create the window class structure
    WNDCLASSEX wc;
    wc.cbSize = sizeof(WNDCLASSEX);

    // fill the struct with info
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC)WinProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = NULL;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = APPTITLE;
    wc.hIconSm = NULL;

    // set up the window with the class info
    return RegisterClassEx(&wc);
}

// Helper function to create the window and refresh it
int InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    // create a new window
    window = CreateWindow(
        APPTITLE,            // window class
        APPTITLE,            // title bar
        WS_OVERLAPPEDWINDOW, // window style
        CW_USEDEFAULT,       // x position of window
        CW_USEDEFAULT,       // y position of window
        640,                 // width of the window
        480,                 // height of the window
        NULL,                // parent window
        NULL,                // menu
        hInstance,           // application instance
        NULL);               // window parameters

    // was there an error creating the window?
    if (window == 0)
        return 0;

    // display the window
    ShowWindow(window, nCmdShow);
    UpdateWindow(window);

    return 1;
}

// Entry point function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // create window
    MyRegisterClass(hInstance);
    if (!InitInstance(hInstance, nCmdShow))
        return 0;

    // initialize the game
    if (!Game_Init())
        return 0;

    // main message loop
    MSG msg;
    while (!gameover)
    {
        // process Windows events
        // LPMSG lpMsg：消息指针
        // HWND hWnd：事件关联的窗口句柄
        // UINT wMsgFilterMin：已收到的第一条消息
        // UINT wMsgFilterMax：已收到的最后一条消息
        // UINT wRemoveMsg：读取消息后消息处理的标志，PM_REMOVE消息队列中移除，PM_NOREMOVE保留。
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

    return msg.wParam;
}
