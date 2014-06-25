#include <string.h>
#include "command.h"

#ifndef RESOLVE_COMMAND
#define RESOLVE_COMMAND(str) if(0 == strcmp(raw, "str")) { return Command_##str; } 
#endif

Command parseCommand(char * raw) {
	RESOLVE_COMMAND(Quit)
	RESOLVE_COMMAND(MoveFocusDown)
	RESOLVE_COMMAND(MoveFocusUp)
	RESOLVE_COMMAND(MoveFocusLeft)
	RESOLVE_COMMAND(MoveFocusRight)
	RESOLVE_COMMAND(MoveWindowDown)
	RESOLVE_COMMAND(MoveWindowUp)
	RESOLVE_COMMAND(MoveWindowLeft)
	RESOLVE_COMMAND(MoveWindowRight)
	RESOLVE_COMMAND(MoveWindowToDesktop)
	return Command_Unknown;
}