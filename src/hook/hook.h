#ifndef HOOK_H
#define HOOK_H

#define SOME_WINDOW_CREATED (WM_USER + 42)

void register_window_hooks(HWND callback_hwnd);
void unregister_window_hooks();

#endif