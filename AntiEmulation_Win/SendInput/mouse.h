#pragma once
#include <windows.h>

//发送鼠标消息
static void MouseClick(const unsigned int x, const unsigned int y, const unsigned char type) 
{
    SetCursorPos(x, y);
    INPUT input;
    ZeroMemory(&input, sizeof(input));
    input.type = INPUT_MOUSE;

    switch (type) 
    {
    case 1:
        input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
        SendInput(1, &input, sizeof(input));
        input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
        break;
    case 2:
        input.mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;
        SendInput(1, &input, sizeof(input));
        input.mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
        break;
    default:
        input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
        SendInput(1, &input, sizeof(input));
        input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    }

    SendInput(1, &input, sizeof(input));
}

//发送鼠标消息
static void MouseClickHard(const unsigned int x, const unsigned int y, const unsigned char type)
{
    INPUT input;
    ZeroMemory(&input, sizeof(input));
    input.type = INPUT_HARDWARE;

    switch (type)
    {
    case 1:
        input.hi.uMsg = WM_RBUTTONDOWN;
        SendInput(1, &input, sizeof(input));
        input.hi.uMsg = WM_RBUTTONUP;
        break;
    case 2:
        input.hi.uMsg = WM_MBUTTONDOWN;
        SendInput(1, &input, sizeof(input));
        input.hi.uMsg = WM_MBUTTONUP;
        break;
    default:
        input.hi.uMsg = WM_LBUTTONDOWN;
        input.hi.wParamL = MAKELPARAM(x, y);
        input.hi.wParamH = MK_LBUTTON;
        SendInput(1, &input, sizeof(input));
        input.hi.uMsg = WM_LBUTTONUP;
        input.hi.wParamL = MAKELPARAM(x, y);
        input.hi.wParamH = 0;
    }

    SendInput(1, &input, sizeof(input));
}