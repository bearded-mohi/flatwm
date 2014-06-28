#ifndef COMMAND_H
#define COMMAND_H

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
	Command_MoveWindowToDesktop1,
	Command_MoveWindowToDesktop2,
	Command_MoveWindowToDesktop3,
	Command_MoveWindowToDesktop4,
	Command_GoToDesktop1,
	Command_GoToDesktop2,
	Command_GoToDesktop3,
	Command_GoToDesktop4,
	Command_EnumWindows,
	Command_Unknown
} Command;

Command parse_command(char *raw);
void dispatch_command(HWND selfHwnd, Command cmd);

#endif