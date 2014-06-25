#ifndef _COMMAND_H_
#define _COMMAND_H_

typedef enum {
	Command_Quit,
	Command_MoveFocusDown,
	Command_MoveFocusUp,
	Command_MoveFocusLeft,
	Command_MoveFocusRight,
	Command_MoveWindowDown,
	Command_MoveWindowUp,
	Command_MoveWindowLeft,
	Command_MoveWindowRight,
	Command_MoveWindowToDesktop,
	Command_EnumWindows,
	Command_Unknown
} Command;

Command parseCommand(char * raw);
void dispatchCommand(HWND selfHwnd, Command cmd);

#endif