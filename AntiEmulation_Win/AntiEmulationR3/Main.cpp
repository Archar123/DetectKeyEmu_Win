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
		bool ret = anti_cheat->protectHWND(main_window->getSelfHandle());
		printf("protectHWND = %d\n", ret);
		main_window->onMessage();
		anti_cheat->unprotectHWND();
	}
	UnstallHook();
	delete main_window;
	return 0;
}
