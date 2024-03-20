#include <windows.h>

const char ProgramTitle[] = "Hello Windows"; // sizeof(ProgramTitle) / sizeof(char), strlen

// The window event callback function
// hWnd：窗口句柄
// wParam,  lParam：参数值的高位和低位
LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;					 // 启动及停止屏幕更新，设备环境加锁
		HDC hdc = BeginPaint(hWnd, &ps); // start drawing，hdc获取程序窗口的设备环境
		for (int n = 0; n < 20; n++)
		{
			int x = n * 20;
			int y = n * 20;
			RECT drawRect = {x, y, x + 100, y + 20};
			DrawText(hdc, ProgramTitle, strlen(ProgramTitle), &drawRect, DT_CENTER);
		}
		EndPaint(hWnd, &ps); // stop drawing
	}
	break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

// Helper function to set up the window properties，设置程序窗口
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	// set the new window's properties
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW; // CS_HREDRAW窗口移动或宽度改变时重绘，CS_VREDRAW窗口高度改变时重绘
	wc.lpfnWndProc = (WNDPROC)WinProc;	// 回调函数指针，Windows消息和hWnd值匹配时，自动被调用
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = ProgramTitle;
	wc.hIconSm = NULL;
	return RegisterClassEx(&wc);
}

// Helper function to create the window and refresh it，创建程序窗口
int InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	// create a new window
	HWND hWnd = CreateWindow(
		ProgramTitle,				  // window class
		ProgramTitle,				  // title bar
		WS_OVERLAPPEDWINDOW,		  // window style
		CW_USEDEFAULT, CW_USEDEFAULT, // position of window
		640, 480,					  // dimensions of the window
		NULL,						  // parent window (not used)
		NULL,						  // menu (not used)
		hInstance,					  // application instance
		NULL);						  // window parameters (not used)

	// was there an error creating the window?
	if (hWnd == 0)
		return 0;

	// display the window
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd); // 发送WM_PAINT消息给窗口处理器

	return 1;
}

// Entry point for a Windows program
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// create the window
	MyRegisterClass(hInstance);
	if (!InitInstance(hInstance, nCmdShow))
		return 0;

	// main message loop
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

// gcc 02.windowTest1.c -o 02.windowTest1 -mwindows && 02.windowTest1
