#include <string.h>
#include <windows.h>

#include "command.h"
#include "layout.h"

#define PARSE_COMMAND(c) if (0 == strcmp(raw, #c)) { return Command_##c; } 

#define DISPATCH_COMMAND(c) if (Command_##c == cmd) { c##Impl(selfHwnd); }

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
	
}

static void MoveWindowUpImpl(HWND selfHwnd) {	
	
}

static void MoveWindowLeftImpl(HWND selfHwnd) {
	MessageBox(NULL, TEXT("MoveWindowLeftImpl"), TEXT("Debug"), MB_OK);
}

static void MoveWindowRightImpl(HWND selfHwnd) {
	MessageBox(NULL, TEXT("MoveWindowRightImpl"), TEXT("Debug"), MB_OK);
}


static void MoveWindowToDesktop1Impl(HWND selfHwnd) {
	move_window_to_desktop(1);
}
static void MoveWindowToDesktop2Impl(HWND selfHwnd) {
	move_window_to_desktop(2);
}
static void MoveWindowToDesktop3Impl(HWND selfHwnd) {
	move_window_to_desktop(3);
}
static void MoveWindowToDesktop4Impl(HWND selfHwnd) {
	move_window_to_desktop(4);
}


static void GoToDesktop1Impl(HWND selfHwnd) {
	go_to_desktop(1);
}
static void GoToDesktop2Impl(HWND selfHwnd) {
	go_to_desktop(2);
}
static void GoToDesktop3Impl(HWND selfHwnd) {
	go_to_desktop(3);
}
static void GoToDesktop4Impl(HWND selfHwnd) {
	go_to_desktop(4);
}


static void EnumWindowsImpl(HWND selfHwnd) {
	layout_list();
}

static void UnknownImpl(HWND selfHwnd) {
	MessageBox(NULL, TEXT("WAT??"), TEXT("Debug"), MB_OK);
}

Command parse_command(char * raw) {
	PARSE_COMMAND(Quit)
	PARSE_COMMAND(MoveFocusDown)
	PARSE_COMMAND(MoveFocusUp)
	PARSE_COMMAND(MoveFocusLeft)
	PARSE_COMMAND(MoveFocusRight)
	PARSE_COMMAND(MoveWindowDown)
	PARSE_COMMAND(MoveWindowUp)
	PARSE_COMMAND(MoveWindowLeft)
	PARSE_COMMAND(MoveWindowRight)
	PARSE_COMMAND(MoveWindowToDesktop1)
	PARSE_COMMAND(MoveWindowToDesktop2)
	PARSE_COMMAND(MoveWindowToDesktop3)
	PARSE_COMMAND(MoveWindowToDesktop4)
	PARSE_COMMAND(GoToDesktop1)
	PARSE_COMMAND(GoToDesktop2)
	PARSE_COMMAND(GoToDesktop3)
	PARSE_COMMAND(GoToDesktop4)
	PARSE_COMMAND(EnumWindows)
	return Command_Unknown;
}

void dispatch_command(HWND selfHwnd, Command cmd) {
	DISPATCH_COMMAND(Quit)
	DISPATCH_COMMAND(MoveFocusDown)
	DISPATCH_COMMAND(MoveFocusUp)
	DISPATCH_COMMAND(MoveFocusLeft)
	DISPATCH_COMMAND(MoveFocusRight)
	DISPATCH_COMMAND(MoveWindowDown)
	DISPATCH_COMMAND(MoveWindowUp)
	DISPATCH_COMMAND(MoveWindowLeft)
	DISPATCH_COMMAND(MoveWindowRight)
	DISPATCH_COMMAND(MoveWindowToDesktop1)
	DISPATCH_COMMAND(MoveWindowToDesktop2)
	DISPATCH_COMMAND(MoveWindowToDesktop3)
	DISPATCH_COMMAND(MoveWindowToDesktop4)
	DISPATCH_COMMAND(GoToDesktop1)
	DISPATCH_COMMAND(GoToDesktop2)
	DISPATCH_COMMAND(GoToDesktop3)
	DISPATCH_COMMAND(GoToDesktop4)
	DISPATCH_COMMAND(EnumWindows)
	DISPATCH_COMMAND(Unknown)
}