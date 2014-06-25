#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "command.h"
#include "config.h"

typedef struct {
	UINT mod;
	UINT vk;
	Command cmd;
} Hotkey;

static Hotkey parseHotkey(char * rawKey, char * rawCmd) {
	Hotkey hk;
	for (char *p = strtok(rawKey, "+"); p != NULL; p = strtok(NULL, "+")) {
		if(0 == strcmp(p, "alt")) {
			hk.mod |= MOD_ALT;
		} else if(0 == strcmp(p, "win")) {
			hk.mod |= MOD_WIN;
		} else if(0 == strcmp(p, "ctrl")) {
			hk.mod |= MOD_CONTROL;
		} else if(0 == strcmp(p, "shift")) {
			hk.mod |= MOD_SHIFT;
		} else if(1 == strlen(p)) {
			hk.vk = VkKeyScan(p[0]) & 0xff;
		}
	}			
	hk.cmd = parseCommand(rawCmd);
	return hk;
}

void applyConfig(const char * filename) {
	FILE * config;
	config = fopen(filename, "rt");
	if(config) {
		char rawKey[256];
		char rawCmd[256];
		while(EOF != (fscanf(config, "%s %s", rawKey, rawCmd))) {
			parseHotkey(rawKey, rawCmd);
		}
		fclose(config);
	}
}

void unregisterHotkeys() {

}