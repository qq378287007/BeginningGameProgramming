#include <windows.h>

void MyRegisterClass(HINSTANCE hInstance)
{
}

int InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    return 1;
}

// hInstance：被调用程序的实例
// hPrevInstance：程序的前一个实例，为NULL时表示程序的第一个实例
// lpCmdLine：命令行参数字符串
// nShowCmd：创建程序窗口时的显示方式
// 非零表示成功，返回零表示程序未进入主循环并提前终止
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    // register the class
    MyRegisterClass(hInstance);

    // initialize application
    if (!InitInstance(hInstance, nShowCmd))
        return FALSE;

    // main message loop
    // LPMSG lpMsg：MSG指针
    // HWND hWnd：特定窗口消息的句柄，NULL则返回当前程序实例的所有消息
    // UNINT wMsgFilterMin和UNINT wMsgFilterMax：返回一定范围内的消息
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        // 两者一起创立WinProc（窗口回调函数）中接受的消息
        TranslateMessage(&msg); // 将虚拟键盘消息翻译成字符消息
        DispatchMessage(&msg);  // 发送回Windows消息系统
    }

    return msg.wParam;
}

// gcc 01.main2.c -o 01.main && 01.main2
