#include <windows.h>

#include "config.h"
#include "command.h"
#include "layout.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch(msg) {
		case WM_CREATE:
			//TODO: read config path from command line arg
			readConfig("_flatwmrc");
			registerHotkeys(hwnd);
			initLayout();
			break;
		case WM_HOTKEY:
			dispatchCommand(hwnd, wParam);
			break;
		case WM_DESTROY:
			unregisterHotkeys(hwnd);
			disposeLayout();
			PostQuitMessage(0);
			return 0;
	}
	return DefWindowProcW(hwnd, msg, wParam, lParam);
}

int CALLBACK WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	MSG msg;
	WNDCLASSEX wx;

	wx.cbSize        = sizeof(WNDCLASSEX);
    wx.style         = 0;
    wx.lpfnWndProc   = WndProc;
    wx.cbClsExtra    = 0;
    wx.cbWndExtra    = 0;
    wx.hInstance     = hInstance;
    wx.hIcon         = 0;
    wx.hCursor       = 0;
    wx.hbrBackground = 0;
    wx.lpszMenuName  = 0;
    wx.lpszClassName = TEXT("flatwm");
    wx.hIconSm       = 0;

	RegisterClassEx(&wx);
	CreateWindowEx(0, wx.lpszClassName, TEXT("Dummy"), 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, NULL, NULL); 	

	while(GetMessage(&msg, NULL, 0, 0)) {
		DispatchMessage(&msg);
	}

	return 0;
}