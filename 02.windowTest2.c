#include <windows.h>

const char ProgramTitle[] = "Hello Windows";

// The window event callback function
LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps); // start drawing
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

// Entry point for a Windows program
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// set the new window's properties
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
		ProgramTitle,
		NULL};
	if (RegisterClassEx(&wc) == 0)
	{
		MessageBox(NULL, "RegisterClassEx Fail!", ProgramTitle, MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}

	// create the window and refresh it
	HWND hWnd = CreateWindow(
		ProgramTitle,				  // window class
		ProgramTitle,				  // title bar
		WS_OVERLAPPEDWINDOW,		  // window style
		CW_USEDEFAULT, CW_USEDEFAULT, // position of window
		640, 480,					  // dimensions of the window
		NULL,						  // GetDesktopWindow()  // parent window (not used)
		NULL,						  // menu (not used)
		hInstance,					  // application instance
		NULL);						  // window parameters (not used)
	if (hWnd == 0)					  // was there an error creating the window?
	{
		MessageBox(NULL, "CreateWindow Fail!", ProgramTitle, MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}
	// display the window
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// main message loop
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	UnregisterClass(ProgramTitle, hInstance);

	return msg.wParam;
}
