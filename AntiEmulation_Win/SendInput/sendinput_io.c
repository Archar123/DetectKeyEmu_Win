#include <stdio.h>
#include "mouse.h"

int main(int argc, char *argv[])
{
	
	HWND hwnd = FindWindowW(NULL, L"Ô­Éñ");
	if (hwnd == NULL)
	{
		printf("get hwnd failed, %d", GetLastError());
		getchar();
		return 0;
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

	return 0;
}