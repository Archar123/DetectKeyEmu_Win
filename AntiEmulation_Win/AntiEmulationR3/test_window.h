#pragma once
#include <windows.h>
#include <stdio.h>
#include <string>

class window
{
	HWND wnd;

	ATOM WRegisterClass(std::string winclass);
public:
	window(std::string name,std::string winclass);
	~window();

	bool onMessage();
	HWND getSelfHandle();
};

