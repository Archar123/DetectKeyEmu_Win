#include <iostream>
#include "AntiEmulationR3.h"
#include "test_window.h"
#include "DetectSendInject.h"

extern c_anti_hack* anti_cheat;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	if (AllocConsole()) 
	{
		freopen("CONOUT$", "w", stdout);
		setlocale(LC_ALL, "CN"); 
	}

	InstallHook();
	window* main_window = new window(std::string("TestGame"), std::string("Game_Class"));

	if (main_window->getSelfHandle())
	{
		//bool ret = anti_cheat->protectHWND(main_window->getSelfHandle());
		//printf("protectHWND = %d\n", ret);
		main_window->onMessage();
		//anti_cheat->unprotectHWND();
	}
	UnstallHook();
	delete main_window;
	return 0;
}

/*

BOOL GetMessageWithTimeout(MSG& msg, UINT to)
{
	BOOL res;
	UINT_PTR timerId = SetTimer(NULL, NULL, to, NULL);
	res = GetMessage(&msg, NULL, 0, 0);
	KillTimer(NULL, timerId);
	if (!res)
		return FALSE;
	if (msg.message == WM_TIMER && msg.hwnd == NULL && msg.wParam == timerId)
		return FALSE;
	return TRUE;
}

void DoCheck(int time)
{
	InstallHook();

	MSG msg = { 0 };
	while (GetMessageWithTimeout(msg, time))
	{
	if (msg.message == WM_TIMER)
	{
		DispatchMessage(&msg);
	}
	else if (msg.message == WM_CLOSE || msg.message == WM_DESTROY)
	{
		util::Flogf("Daemon Thread Exist.");
		break;
	}
	}

	UnstallHook();

	DoCheck(time);
}

 */