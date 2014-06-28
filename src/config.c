#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <assert.h>

#include "command.h"
#include "config.h"

#define MAX_BINDINGS 256

typedef struct {
	UINT mod;
	UINT vk;
	Command cmd;
} Hotkey;

static Hotkey _bindings[MAX_BINDINGS];
static int _bindings_count = 0;

static Hotkey parse_hotkey(char *raw_key, char *raw_cmd) {
	Hotkey hk = { 
		.mod = 0,
		.vk  = 0,
		.cmd = Command_Unknown
	};
	for (char *p = strtok(raw_key, "+"); p != NULL; p = strtok(NULL, "+")) {
		if (0 == strcmp(p, "alt")) {
			hk.mod |= MOD_ALT;
		} else if (0 == strcmp(p, "win")) {
			hk.mod |= MOD_WIN;
		} else if (0 == strcmp(p, "ctrl")) {
			hk.mod |= MOD_CONTROL;
		} else if (0 == strcmp(p, "shift")) {
			hk.mod |= MOD_SHIFT;
		} else if (1 == strlen(p)) {
			//TODO: recognize arrows like up, down, left, right
			hk.vk = VkKeyScan(p[0]) & 0xff;
		}
	}			
	hk.cmd = parse_command(raw_cmd);
	return hk;
}

void read_config(const char *filename) {
	FILE *config;
	config = fopen(filename, "rt");
	if (config) {
		char raw_key[256];
		char raw_cmd[256];
		while (EOF != (fscanf(config, "%s %s", raw_key, raw_cmd))) {
			if (_bindings_count < MAX_BINDINGS - 1) {
				_bindings[_bindings_count++] = parse_hotkey(raw_key, raw_cmd);	
			} else {
				// TODO: something
			}
			
		}
		fclose(config);
	}
}

void register_hotkeys(HWND hwnd) {
	for (int i = 0; i < _bindings_count; i++) {
		RegisterHotKey(hwnd, _bindings[i].cmd, _bindings[i].mod, _bindings[i].vk);
	}
}

void unregister_hotkeys(HWND hwnd) {
	for (int i = 0; i < _bindings_count; i++) {
		UnregisterHotKey(hwnd, _bindings[i].cmd);
	}
}