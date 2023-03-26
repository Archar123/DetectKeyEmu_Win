#include <iostream>
#include <windows.h>

int main()
{
    std::cout << "Test SendMessage!\n";
    HWND hwnd = FindWindowW(L"Game_Class", NULL);
    if (hwnd == NULL)
    {
        printf("get hwnd failed, %d", GetLastError());
        getchar();
        return 0;
    }

    while (true)
    {
        LRESULT ret = SendMessageA(hwnd, BM_CLICK, 100, 100);
        printf("sendmessage. %d", ret);
        getchar();
    }

    return 0;
}