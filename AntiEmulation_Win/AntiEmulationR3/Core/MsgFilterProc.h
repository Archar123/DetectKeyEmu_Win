#pragma once

LRESULT CALLBACK ProcFilter(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch (message) {
			//Mouse
		case WM_LBUTTONDOWN: { 
			if (!anti_cheat->MouseButtonsArray[0]) {
				printf("Device Emulation [SendMessage]\n");
				return 0;
			}
			break;
		}
		case WM_MBUTTONDOWN: {
			if (!anti_cheat->MouseButtonsArray[1]) {
				printf("Device Emulation [SendMessage]\n");
				return 0;
			}
			break;
		}
		case WM_RBUTTONDOWN: {
			if (!anti_cheat->MouseButtonsArray[2]) {			
				printf("Device Emulation [SendMessage]\n");
				return 0;
			}
			break;
		}

            //KeyBoard
		case WM_KEYDOWN: { 
			if (!anti_cheat->KeyBoardArray[wParam].Key || anti_cheat->KeyBoardArray[wParam].Block_WM_KEY_DOWN_counter) {
				if (anti_cheat->KeyBoardArray[wParam].Block_WM_KEY_DOWN_counter) {
					anti_cheat->KeyBoardArray[wParam].Block_WM_KEY_DOWN_counter--;
				}
				printf("Device Emulation [SendMessage]\n");
				return 0;
			}
			break;
		}
		case WM_KEYUP: { 
			if (anti_cheat->KeyBoardArray[wParam].Block_WM_KEY_UP_counter) {
				anti_cheat->KeyBoardArray[wParam].Block_WM_KEY_UP_counter--;
				printf("Device Emulation [SendMessage]\n");
				return 0;
			}
			break;
		}


		case WM_INPUT: {
			UINT dwSize = sizeof(RAWINPUT);
			BYTE *lpb = NULL;
			RAWINPUT raw_i;

			if (GET_RAWINPUT_CODE_WPARAM(wParam) == RIM_INPUT) {
				GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
				lpb = new BYTE[dwSize];

				if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
					return 0;

				if (((RAWINPUT*)(lpb))->header.hDevice) {
					delete[] lpb;
				}
				else {
					if (((RAWINPUT*)(lpb))->data.keyboard.Message == WM_KEYDOWN) {
						anti_cheat->KeyBoardArray[((RAWINPUT*)(lpb))->data.keyboard.VKey].Block_WM_KEY_DOWN_counter++;
					}
					if (((RAWINPUT*)(lpb))->data.keyboard.Message == WM_KEYUP) {
						anti_cheat->KeyBoardArray[((RAWINPUT*)(lpb))->data.keyboard.VKey].Block_WM_KEY_UP_counter++;
					}
					delete[] lpb;
					printf("Device Emulation [SendInput]\n");
					return 0;
				}
			}
			break;
		}

		default: break;
	}

	
	WNDPROC OriginalProc = anti_cheat->getMsgProc();
	if (anti_cheat->getMsgProc()) 
	{
		OriginalProc(hWnd, message, wParam, lParam);
	}
	else 
	{
		return 0;
	}
}
