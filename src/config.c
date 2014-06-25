#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "command.h"
#include "config.h"

#define MAX_BINDINGS 256

typedef struct {
	UINT mod;
	UINT vk;
	Command cmd;
} Hotkey;

static Hotkey _bindings[MAX_BINDINGS];
static int _bindingsCount = 0;

static Hotkey parseHotkey(char * rawKey, char * rawCmd) {
	Hotkey hk = { mod: 0, vk: 0, cmd: Command_Unknown };
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

void readConfig(const char * filename) {
	FILE * config;
	config = fopen(filename, "rt");
	if(config) {
		char rawKey[256];
		char rawCmd[256];
		while(EOF != (fscanf(config, "%s %s", rawKey, rawCmd))) {
			if(_bindingsCount < MAX_BINDINGS - 1) {
				_bindings[_bindingsCount++] = parseHotkey(rawKey, rawCmd);	
			} else {
				// TODO: something
			}
			
		}
		fclose(config);
	}
}

void registerHotkeys(HWND hwnd) {
	for(int i = 0; i < _bindingsCount; i++) {
		RegisterHotKey(hwnd, _bindings[i].cmd, _bindings[i].mod, _bindings[i].vk);
	}
}

void unregisterHotkeys(HWND hwnd) {
	for(int i = 0; i < _bindingsCount; i++) {
		UnregisterHotKey(hwnd, _bindings[i].cmd);
	}
}