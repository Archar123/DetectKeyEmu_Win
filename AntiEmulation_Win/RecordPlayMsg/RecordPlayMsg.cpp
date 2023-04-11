// RecordPlayMsg.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <windows.h>


typedef BOOL(__stdcall* MR_StartRecord)(HWND ahWnd, int nOnlyKeyborad);
typedef BOOL(__stdcall* MR_StopRecord)();

typedef BOOL(__stdcall* MR_StartPlayBack)(HWND ahWnd, int nDoClear);
typedef BOOL(__stdcall* MR_StopPlayBack)();




int main()
{
    std::cout << "Hello World!\n";

    do
    {
        HMODULE mod = LoadLibraryW(L"Msg.dll");
        if (mod == NULL)
        {
            printf("load error=%d \r\n", GetLastError());
            break;
        }

        MR_StartRecord start_record = (MR_StartRecord)GetProcAddress(mod, "MR_StartRecord");
        MR_StopRecord stop_record = (MR_StopRecord)GetProcAddress(mod, "MR_StopRecord");

        MR_StartPlayBack start_play = (MR_StartPlayBack)GetProcAddress(mod, "MR_StartPlayBack");
        MR_StopPlayBack stop_play = (MR_StopPlayBack)GetProcAddress(mod, "MR_StopPlayBack");

        if (!start_play || !stop_play || !start_record || !stop_record)
        {
            printf("get address error. \r\n");
            break;
        }

        HWND hwnd = GetDesktopWindow();
        if (hwnd==NULL)
        {
            printf("get GetDesktopWindow error.=%d \r\n", GetLastError());
            break;
        }

        printf("start record. \r\n");
        start_record(hwnd, false);

        for (int i = 0; i < 10; i++)
        {
            Sleep(1000);
            printf(".");
        }

        printf(">>>finish record. \r\n");
        stop_record();

        /// <summary>
        /// /////////////////////////////////////////////////////////////////////
        /// </summary>
        /// <returns></returns>
        printf("press any key to playback. \r\n");
        getchar();

        start_play(hwnd, true);

        printf("press any key to stop playback. \r\n");
        getchar();

        stop_play();

    } while (false);

    getchar();
}

