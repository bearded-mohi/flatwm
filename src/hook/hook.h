#ifndef _HOOK_H_
#define _HOOK_H_

void registerWindowHooks(void (*onCreateWindow)(HWND hwnd), void(*onDestroyWindow)(HWND hwnd));

void unregisterWindowHooks();

#endif