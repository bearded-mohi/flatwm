#include <windows.h>
#include <assert.h>

#include "config.h"
#include "command.h"
#include "layout.h"

static LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
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
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASS wc = {
	    .style         = 0,
	    .lpfnWndProc   = wndProc,
	    .cbClsExtra    = 0,
	    .cbWndExtra    = 0,
	    .hInstance     = hInstance,
	    .hIcon         = 0,
	    .hCursor       = 0,
	    .hbrBackground = 0,
	    .lpszMenuName  = 0,
	    .lpszClassName = TEXT("flatwm"),
	};
	
	RegisterClass(&wc);
	CreateWindow(wc.lpszClassName, TEXT("Dummy"), 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, hInstance, NULL);

	MSG msg;
	while(GetMessage(&msg, NULL, 0, 0)) {
		DispatchMessage(&msg);
	}

	return 0;
}