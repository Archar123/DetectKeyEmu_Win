#include <windows.h>
#include <vector>
#include "Inc.h"

class c_anti_hack 
{
	DWORD init_state = 0;
	bool IsDebugger = false;
	BOOL IsWow64;
	DWORD ProcessId = GetCurrentProcessId();

	HHOOK h_winhook = 0;

	BYTE ObjectTypeNumberOfThread = 0xFF;
	BYTE ObjectTypeNumberOfProcess = 0xFF;

	WNDPROC ProcOriginal = 0;

	LPVOID dll_notify_cookie = 0;

	HWND wnd_protected = 0;

	std::vector<LPVOID> suspected_threadbase;
	

	bool init_fakemsg_profiler();
	bool init_fakemsg_oldprofiler();
	void uninit_fakemsg_profiler();
	void uninit_fakemsg_oldprofiler();

public:
	bool		MouseButtonsArray[3];
	__keyallow  KeyBoardArray[256];

	bool protectHWND(HWND wnd);
	void unprotectHWND();

	DWORD   getState() { return this->init_state; }
	WNDPROC getMsgProc() { return this->ProcOriginal; }

	c_anti_hack();
	~c_anti_hack();
};

/*
 new anti-sendmessage works only on windows 10 x64
 old anti-sendmessage works everywhere
 sendinput works everywhere
*/