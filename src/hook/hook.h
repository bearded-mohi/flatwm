#ifndef HOOK_H
#define HOOK_H

#define SOME_WINDOW_CREATED (WM_USER + 42)
#define SOME_WINDOW_DESTROYED (WM_USER + 43)

void register_window_hooks(HWND callback_hwnd);
void unregister_window_hooks();

#endif