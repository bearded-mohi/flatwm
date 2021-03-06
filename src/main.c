#include <windows.h>
#include <assert.h>

#include "log.h"
#include "config.h"
#include "command.h"
#include "layout.h"
#include "hook/hook.h"

static BOOL _system_animations;

static void disable_system_animations() {
	ANIMATIONINFO ai;
	ai.cbSize = sizeof(ANIMATIONINFO); 
	SystemParametersInfo(SPI_GETANIMATION, sizeof(ANIMATIONINFO), &ai, 0); 
	_system_animations = ai.iMinAnimate; 

	if (_system_animations) { 
		ai.iMinAnimate = FALSE; 
		SystemParametersInfo(SPI_SETANIMATION, sizeof(ANIMATIONINFO), &ai, 0); 
	} 	
}

static void restore_system_animations() {
	ANIMATIONINFO ai;
	ai.cbSize = sizeof(ANIMATIONINFO); 
	if (_system_animations) { 
		ai.iMinAnimate = TRUE; 
		SystemParametersInfo(SPI_SETANIMATION, sizeof(ANIMATIONINFO), &ai, 0); 
	} 		
}

static LRESULT CALLBACK wnd_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
		case WM_CREATE:
			//TODO: read config path from command line arg
			//TODO: make default config in code
			//TODO: write default config to disk if no config supplied
			log_init("_log");
			read_config("_flatwmrc");
			register_window_hooks(hwnd);
			register_hotkeys(hwnd);
			disable_system_animations();
			layout_init();
			break;
		case SOME_WINDOW_CREATED:
			layout_track((HWND) wParam);
			break;
		case SOME_WINDOW_DESTROYED:
			layout_untrack((HWND) wParam);
			break;
		case WM_HOTKEY:
			dispatch_command(hwnd, wParam);
			break;
		case WM_DESTROY:
			unregister_window_hooks();
			unregister_hotkeys(hwnd);			
			restore_system_animations();
			layout_dispose();
			log_dispose();
			PostQuitMessage(0);
			return 0;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

int CALLBACK WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASS wc = {
	    .style         = 0,
	    .lpfnWndProc   = wnd_proc,
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
	while (GetMessage(&msg, NULL, 0, 0)) {
		DispatchMessage(&msg);
	}

	return 0;
}