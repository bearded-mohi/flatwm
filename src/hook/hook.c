#include <windows.h>
#include <assert.h>

#include "hook.h"

#define SHARED __attribute__((section(".shr"), shared))
#define UNSHARED


//TODO: use both!

#define CBT_HOOK
// #define SHELL_HOOK

static HHOOK _shell_hook    UNSHARED = NULL;
static HWND  _callback_hwnd SHARED   = NULL;

static LRESULT CALLBACK hook_proc(int nCode, WPARAM wParam, LPARAM lParam) {
    
    #ifdef SHELL_HOOK
	if (HSHELL_WINDOWCREATED == nCode) {
        PostMessage(_callback_hwnd, SOME_WINDOW_CREATED, wParam, 0);
	} else if (HSHELL_WINDOWDESTROYED == nCode) {
        PostMessage(_callback_hwnd, SOME_WINDOW_DESTROYED, wParam, 0);
	}
    #endif

    #ifdef CBT_HOOK
    if (HCBT_CREATEWND == nCode) {
        LPCBT_CREATEWND cs  = (LPCBT_CREATEWND) lParam;
        
        BOOL is_parentless  = (0 == cs->lpcs->hwndParent);
        BOOL is_overlapped  = WS_OVERLAPPED == (WS_OVERLAPPED & cs->lpcs->style);
        BOOL is_app_window  = WS_EX_APPWINDOW == (WS_EX_APPWINDOW & cs->lpcs->dwExStyle);
        BOOL is_window_edge = WS_EX_WINDOWEDGE == (WS_EX_WINDOWEDGE & cs->lpcs->dwExStyle);
        
        if (is_overlapped && is_window_edge) {
            PostMessage(_callback_hwnd, SOME_WINDOW_CREATED, wParam, 0);
        }
    } else if (HCBT_DESTROYWND == nCode) {                
        PostMessage(_callback_hwnd, SOME_WINDOW_DESTROYED, wParam, 0);   
    }
    #endif

	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

void register_window_hooks(HWND callback_hwnd) {
	if (!_shell_hook) {  
        _callback_hwnd = callback_hwnd;   
		
        #ifdef SHELL_HOOK
        _shell_hook = SetWindowsHookEx(WH_SHELL, hook_proc, GetModuleHandle("hook"), 0);
        #endif

        #ifdef CBT_HOOK
        _shell_hook = SetWindowsHookEx(WH_CBT, hook_proc, GetModuleHandle("hook"), 0);
        #endif
	}
}

void unregister_window_hooks() {
	if(_shell_hook) {
		UnhookWindowsHookEx(_shell_hook);
        _callback_hwnd = NULL;
	}    
}