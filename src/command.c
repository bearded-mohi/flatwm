#include <string.h>
#include <windows.h>
#include "command.h"

#ifndef PARSE_COMMAND
#define PARSE_COMMAND(c) if(0 == strcmp(raw, #c)) { return Command_##c; } 
#endif

#ifndef DISPATCH_COMMAND
#define DISPATCH_COMMAND(c) if(Command_##c == cmd) { c##Impl(selfHwnd); }
#endif

static void QuitImpl(HWND selfHwnd) {
	SendMessage(selfHwnd, WM_DESTROY, 0, 0);
}

static void MoveFocusDownImpl(HWND selfHwnd) {
	MessageBox(NULL, TEXT("MoveFocusDownImpl"), TEXT("Debug"), MB_OK);
}

static void MoveFocusUpImpl(HWND selfHwnd) {
	MessageBox(NULL, TEXT("MoveFocusUpImpl"), TEXT("Debug"), MB_OK);
}

static void MoveFocusLeftImpl(HWND selfHwnd) {
	MessageBox(NULL, TEXT("MoveFocusLeftImpl"), TEXT("Debug"), MB_OK);
}

static void MoveFocusRightImpl(HWND selfHwnd) {
	MessageBox(NULL, TEXT("MoveFocusRightImpl"), TEXT("Debug"), MB_OK);
}

static void MoveWindowDownImpl(HWND selfHwnd) {
	MessageBox(NULL, TEXT("MoveWindowDown"), TEXT("Debug"), MB_OK);
}

static void MoveWindowUpImpl(HWND selfHwnd) {
	MessageBox(NULL, TEXT("MoveWindowUpImpl"), TEXT("Debug"), MB_OK);
}

static void MoveWindowLeftImpl(HWND selfHwnd) {
	MessageBox(NULL, TEXT("MoveWindowLeftImpl"), TEXT("Debug"), MB_OK);
}

static void MoveWindowRightImpl(HWND selfHwnd) {
	MessageBox(NULL, TEXT("MoveWindowRightImpl"), TEXT("Debug"), MB_OK);
}

static void MoveWindowToDesktopImpl(HWND selfHwnd) {
	MessageBox(NULL, TEXT("MoveWindowToDesktopImpl"), TEXT("Debug"), MB_OK);
}

static BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
	char caption[256];
	GetWindowText(hwnd, caption, 256);
	MessageBox(NULL, caption, TEXT("Debug"), MB_OK);
	return TRUE;
}

static void EnumWindowsImpl(HWND selfHwnd) {
	EnumWindows(EnumWindowsProc, 0);
}

static void UnknownImpl(HWND selfHwnd) {
	MessageBox(NULL, TEXT("WAT??"), TEXT("Debug"), MB_OK);
}

Command parseCommand(char * raw) {
	PARSE_COMMAND(Quit)
	PARSE_COMMAND(MoveFocusDown)
	PARSE_COMMAND(MoveFocusUp)
	PARSE_COMMAND(MoveFocusLeft)
	PARSE_COMMAND(MoveFocusRight)
	PARSE_COMMAND(MoveWindowDown)
	PARSE_COMMAND(MoveWindowUp)
	PARSE_COMMAND(MoveWindowLeft)
	PARSE_COMMAND(MoveWindowRight)
	PARSE_COMMAND(MoveWindowToDesktop)
	PARSE_COMMAND(EnumWindows)
	return Command_Unknown;
}

void dispatchCommand(HWND selfHwnd, Command cmd) {
	DISPATCH_COMMAND(Quit)
	DISPATCH_COMMAND(MoveFocusDown)
	DISPATCH_COMMAND(MoveFocusUp)
	DISPATCH_COMMAND(MoveFocusLeft)
	DISPATCH_COMMAND(MoveFocusRight)
	DISPATCH_COMMAND(MoveWindowDown)
	DISPATCH_COMMAND(MoveWindowUp)
	DISPATCH_COMMAND(MoveWindowLeft)
	DISPATCH_COMMAND(MoveWindowRight)
	DISPATCH_COMMAND(MoveWindowToDesktop)
	DISPATCH_COMMAND(EnumWindows)
	DISPATCH_COMMAND(Unknown)
}