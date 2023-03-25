#pragma once
#define ANTIHACK_MSG   2
#define ANTIHACK_MSG_1 4


struct __keyallow
{
	bool Key;
	DWORD Block_WM_KEY_DOWN_counter;
	DWORD Block_WM_KEY_UP_counter;
};

