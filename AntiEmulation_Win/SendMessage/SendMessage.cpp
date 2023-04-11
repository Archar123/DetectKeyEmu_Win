#include <iostream>
#include <windows.h>

void ClickButtun(HWND handle, int x, int y)
{
    ::SendMessage(handle, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(x, y));
    ::SendMessage(handle, WM_LBUTTONUP, NULL, MAKELPARAM(x, y));
}

void MouseMove(HWND handle, int x, int y)
{
    ::SendMessage(handle, WM_MOUSEMOVE, 0, MAKELPARAM(x, y));
}

int main()
{
    std::cout << "Test SendMessage!\n";
    //HWND hwnd = FindWindowW(L"Game_Class", NULL);

    HWND hwnd = FindWindowW(NULL, L"原神");
    if (hwnd == NULL)
    {
        printf("get hwnd failed, %d", GetLastError());
        getchar();
        return 0;
    }

    while (true)
    {
        //LRESULT ret = SendMessageA(hwnd, BM_CLICK, 100, 100);

        Sleep(1000);

        for (int i = 0; i < 10 ; i++)
        {
            MouseMove(hwnd, 100*i, 100*i);
            Sleep(100);
        }

        ClickButtun(hwnd, 1000, 1000);
        //getchar();
    }

    return 0;
}