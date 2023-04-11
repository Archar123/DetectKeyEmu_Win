#include <stdio.h>
#include "mouse.h"
HHOOK hook;

MSLLHOOKSTRUCT mouseStruct;

LRESULT CALLBACK LLMP(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode >= 0)
	{
		mouseStruct = *((MSLLHOOKSTRUCT*)lParam);

		if ((mouseStruct.flags & LLMHF_INJECTED) != 0)
			mouseStruct.flags &= ~LLMHF_INJECTED;

		if ((mouseStruct.flags & LLMHF_LOWER_IL_INJECTED) != 0)
			mouseStruct.flags &= ~LLMHF_LOWER_IL_INJECTED;
	}

	return CallNextHookEx(hook, nCode, wParam, lParam);
}


int main(int argc, char *argv[])
{
	if (!(hook = SetWindowsHookEx(WH_MOUSE_LL, LLMP, NULL, 0)))
	{
		MessageBox(NULL, L"failed to install hook", L"fail", MB_ICONERROR);
	}

	//printf("press any key to emun. \r\n");
	//getchar();

	HWND hwnd = NULL;

	while (hwnd == NULL)
	{
		hwnd = FindWindowW(NULL, L"Ô­Éñ");
		Sleep(1000);
	}

    if (!SetForegroundWindow(hwnd))
    {
		printf("SetForegroundWindow failed, %d", GetLastError());
		getchar();
		return 0;
    }
    ;
	while (1)
	{
		for (int i = 0; i < 100; i++)
		{
			SetCursorPos(10 * i, 10 * i); 
			Sleep(10);
		}

		MouseClickHard(1000, 1000, 0);

		mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 1000, 1000, 0, 0);
		//getchar();

		Sleep(5000);
	}

	UnhookWindowsHookEx(hook);
	return 0;
}