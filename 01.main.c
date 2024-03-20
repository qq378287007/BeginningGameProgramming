#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    return MessageBox(NULL, "Welcome to Windows Programming!", "HELLO WORLD", MB_OK | MB_ICONINFORMATION);
}

// gcc 01.main.c -o 01.main && 01.main
