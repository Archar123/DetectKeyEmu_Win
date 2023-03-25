#include "AntiEmulationR3.h"

c_anti_hack* anti_cheat = new c_anti_hack();

#include "MsgFilterProc.h"
#include "DetectSendInput.h"
#include "DetectSendMessage.h"

c_anti_hack::c_anti_hack() 
{
	ZeroMemory(KeyBoardArray, sizeof(KeyBoardArray));
	ZeroMemory(MouseButtonsArray, sizeof(MouseButtonsArray));

#ifdef _M_IX86 
	IsWow64Process(GetCurrentProcess(), &this->IsWow64);
#else
	this->IsWow64 = true;
#endif

	this->init_fakemsg_profiler();

	HMODULE kernel32 = GetModuleHandle(L"kernel32.dll");
	HMODULE kernelbase = GetModuleHandle(L"kernelbase.dll");

	this->suspected_threadbase.push_back(GetProcAddress(kernel32, "LoadLibraryA"));
	this->suspected_threadbase.push_back(GetProcAddress(kernel32, "LoadLibraryW"));
	this->suspected_threadbase.push_back(GetProcAddress(kernel32, "ExitProcess"));
	this->suspected_threadbase.push_back(GetProcAddress(kernel32, "FreeLibrary"));

	if (kernelbase) 
	{
		this->suspected_threadbase.push_back(GetProcAddress(kernelbase, "LoadLibraryA"));
		this->suspected_threadbase.push_back(GetProcAddress(kernelbase, "LoadLibraryW"));
		this->suspected_threadbase.push_back(GetProcAddress(kernelbase, "ExitProcess"));
		this->suspected_threadbase.push_back(GetProcAddress(kernelbase, "FreeLibrary"));
	}
}

c_anti_hack::~c_anti_hack() 
{
	if (this->init_state & ANTIHACK_MSG || this->init_state & ANTIHACK_MSG_1) 
	{
		this->uninit_fakemsg_profiler();
	}
}

void NTAPI anticheat_thread_profiler(void* dll, DWORD reason, void* reserved) {

	switch (reason) 
	{
	case DLL_THREAD_ATTACH: 
	{
		break;
	}
	case DLL_THREAD_DETACH: {
		break;
	}

	case DLL_PROCESS_ATTACH: {
		if (!anti_cheat) {
		}
		break;
	}
	case DLL_PROCESS_DETACH: {
		if (anti_cheat) {
			anti_cheat->~c_anti_hack();
		}
		break;
	}
	}
}

#ifdef _WIN64
#pragma comment (linker, "/INCLUDE:_tls_used")  
#pragma comment (linker, "/INCLUDE:tls_callback_func") 
#else
#pragma comment (linker, "/INCLUDE:__tls_used")
#pragma comment (linker, "/INCLUDE:_tls_callback_func")
#endif

#ifdef _WIN64
#pragma const_seg(".CRT$XLF")
EXTERN_C const
#else
#pragma data_seg(".CRT$XLF")

EXTERN_C
#endif
PIMAGE_TLS_CALLBACK tls_callback_func = anticheat_thread_profiler;
#ifdef _WIN64
#pragma const_seg()
#else
#pragma data_seg()
#endif 