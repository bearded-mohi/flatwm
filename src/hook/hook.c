#include <windows.h>
#include <assert.h>

#include "hook.h"

#define SHARED __attribute__((section(".shr"), shared))

static HHOOK _shell_hook           = NULL;
static HWND  _callback_hwnd SHARED = NULL;

static LRESULT CALLBACK hook_proc(int nCode, WPARAM wParam, LPARAM lParam) {
	// if(HSHELL_WINDOWCREATED == nCode) {
 //        Beep( 750, 300 );
	// 	_onCreateWindow((HWND)wParam);
	// } else if(HSHELL_WINDOWDESTROYED == nCode) {
	// 	_onDestroyWindow((HWND)wParam);
	// }
    
    if (HCBT_CREATEWND == nCode) {
        LPCBT_CREATEWND cs = (LPCBT_CREATEWND) lParam;
        if (0 == cs->lpcs->hwndParent) {
            PostMessage(_callback_hwnd, SOME_WINDOW_CREATED, wParam, 0);
        }
    } else if (HCBT_DESTROYWND == nCode) {                
        //_onDestroyWindow((HWND)wParam);   
    }
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

void register_window_hooks(HWND callback_hwnd) {
	if (!_shell_hook) {  
        _callback_hwnd = callback_hwnd;   
		// _shellHook = SetWindowsHookEx(WH_SHELL, _hookProc, GetModuleHandle("hook"), 0);
        _shell_hook = SetWindowsHookEx(WH_CBT, hook_proc, GetModuleHandle("hook"), 0);
	}
}

void unregister_window_hooks() {
	if(_shell_hook) {
		UnhookWindowsHookEx(_shell_hook);
        _callback_hwnd = NULL;
	}    
}