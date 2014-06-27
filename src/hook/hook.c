#include <windows.h>
#include <assert.h>

static HHOOK _shellHook;
static void (*_onCreateWindow)(HWND hwnd);
static void (*_onDestroyWindow)(HWND hwnd);

static LRESULT CALLBACK _hookProc(int nCode, WPARAM wParam, LPARAM lParam) {
	// if(HSHELL_WINDOWCREATED == nCode) {
 //        Beep( 750, 300 );
	// 	_onCreateWindow((HWND)wParam);
	// } else if(HSHELL_WINDOWDESTROYED == nCode) {
	// 	_onDestroyWindow((HWND)wParam);
	// }
    LPCBT_CREATEWND cs;
    if(HCBT_CREATEWND == nCode) {
        cs = (LPCBT_CREATEWND)lParam;
        if(0 == cs->lpcs->hwndParent) {
            _onCreateWindow((HWND)wParam);
        }
    } else if(HCBT_DESTROYWND == nCode) {                
        //_onDestroyWindow((HWND)wParam);   
    }
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

void registerWindowHooks(void (*onCreateWindow)(HWND hwnd), void(*onDestroyWindow)(HWND hwnd)) {
	if(!_shellHook) {        
        _onCreateWindow = onCreateWindow;
        _onDestroyWindow = onDestroyWindow;
		// _shellHook = SetWindowsHookEx(WH_SHELL, _hookProc, GetModuleHandle("hook"), 0);
        _shellHook = SetWindowsHookEx(WH_CBT, _hookProc, GetModuleHandle("hook"), 0);
	}
}

void unregisterWindowHooks() {
	if(_shellHook) {
		UnhookWindowsHookEx(_shellHook);
        _onCreateWindow = NULL;
        _onDestroyWindow = NULL;
	}    
}