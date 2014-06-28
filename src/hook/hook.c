#include <windows.h>
#include <assert.h>

static HHOOK _shell_hook;

static LRESULT CALLBACK hook_proc(int nCode, WPARAM wParam, LPARAM lParam) {
	// if(HSHELL_WINDOWCREATED == nCode) {
 //        Beep( 750, 300 );
	// 	_onCreateWindow((HWND)wParam);
	// } else if(HSHELL_WINDOWDESTROYED == nCode) {
	// 	_onDestroyWindow((HWND)wParam);
	// }
    LPCBT_CREATEWND cs;
    if (HCBT_CREATEWND == nCode) {
        cs = (LPCBT_CREATEWND)lParam;
        if (0 == cs->lpcs->hwndParent) {
            // _onCreateWindow((HWND)wParam);
        }
    } else if (HCBT_DESTROYWND == nCode) {                
        //_onDestroyWindow((HWND)wParam);   
    }
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

void register_window_hooks() {
	if (!_shell_hook) {        
		// _shellHook = SetWindowsHookEx(WH_SHELL, _hookProc, GetModuleHandle("hook"), 0);
        _shell_hook = SetWindowsHookEx(WH_CBT, hook_proc, GetModuleHandle("hook"), 0);
	}
}

void unregister_window_hooks() {
	if(_shell_hook) {
		UnhookWindowsHookEx(_shell_hook);
	}    
}