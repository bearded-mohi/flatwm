#include <windows.h>
#include "config.h"
#include "command.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch(msg) {
		case WM_CREATE:
			//TODO: read config path from command line arg
			readConfig("_flatwmrc");
			registerHotkeys(hwnd);
			break;
		case WM_HOTKEY:
			dispatchCommand(hwnd, wParam);
			break;
		case WM_DESTROY:
			unregisterHotkeys(hwnd);
			PostQuitMessage(0);
			return 0;
	}
	return DefWindowProcW(hwnd, msg, wParam, lParam);
}

int CALLBACK WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	MSG msg;
	HWND hwnd;
	WNDCLASSW wc;

	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.lpszClassName = L"Window";
	wc.hInstance     = hInstance;
	wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	wc.lpszMenuName  = NULL;
	wc.lpfnWndProc   = WndProc;
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassW(&wc);
	hwnd = CreateWindowW( wc.lpszClassName, L"Window",
	            WS_OVERLAPPEDWINDOW | WS_VISIBLE,
	            100, 100, 350, 250, NULL, NULL, hInstance, NULL);  

	//TODO: hide window
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);				

	while(GetMessage(&msg, NULL, 0, 0)) {
		DispatchMessage(&msg);
	}

	return 0;
}