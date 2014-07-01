#include <stdio.h>
#include <stdbool.h>
#include <windows.h>

#include "log.h"
#include "tile.h"
#include "tile_list.h"
#include "layout.h"

static BOOL is_window_managable(HWND hwnd);

static List *_tiles;

static int _active_desktop = 1;

void layout_track(HWND hwnd) {			
	if (!list_has(_tiles, hwnd)) {
		char caption[256];
		char class_name[256];
		GetWindowText(hwnd, caption, 256);
		GetClassName(hwnd, class_name, 256);
		list_add(_tiles, tile_create(hwnd, class_name, caption, _active_desktop));		
	}			
}

void layout_untrack(HWND hwnd) {
	list_remove(_tiles, hwnd);
}

//TODO: use the same filter as hook use
static BOOL is_window_managable(HWND hwnd) {
	TITLEBARINFO ti;
    HWND hwndTry = NULL;
    HWND hwndWalk = NULL;
    if(!IsWindowVisible(hwnd)) {
        return FALSE;
    }
    hwndTry = GetAncestor(hwnd, GA_ROOTOWNER);
    while(hwndTry != hwndWalk) 
    {
        hwndWalk = hwndTry;
        hwndTry = GetLastActivePopup(hwndWalk);
        if(IsWindowVisible(hwndTry)) 
            break;
    }
    if(hwndWalk != hwnd) {
        return FALSE;
    }    
    ti.cbSize = sizeof(ti);
    GetTitleBarInfo(hwnd, &ti);
    //TODO: why STATE_SYSTEM_INVISIBLE is undefined for winxp?
    // if(ti.rgstate[0] & STATE_SYSTEM_INVISIBLE) {
    //     return FALSE;
    // }
    if(GetWindowLong(hwnd, GWL_EXSTYLE) & WS_EX_TOOLWINDOW) {
        return FALSE;
    }
    return TRUE;
}

static BOOL CALLBACK enum_windows_proc(HWND hwnd, LPARAM lParam) {
	//TODO: exclude is_window_managable in separete filter
	if (is_window_managable(hwnd)) {
		layout_track(hwnd);
	}	
	return TRUE;
}

void layout_list() {
	size_t total_len = 0;	
	foreach_tile(_tiles, t,
		total_len += strlen(tile_get_class_name(t)) + 1;
		total_len += strlen(tile_get_caption(t)) + 1;
	);
	
	char *total_list = malloc(total_len + 1);
	int pos = 0;
	int res = 0;
	foreach_tile(_tiles, t,
		res = snprintf(total_list + pos, total_len, "%s-%s\n", tile_get_class_name(t), tile_get_caption(t));
		if (0 <= res) {
			pos += res;
		} else {
			log_print("snprintf return error");
		}
	);

	MessageBox(NULL, total_list, "tiles list", MB_OK);
	free(total_list);
}

void move_window_to_desktop(int n) {
	if (n == _active_desktop) return;

	HWND foreground = GetForegroundWindow();
	foreach_tile(_tiles, t,
		if (tile_get_hwnd(t) == foreground) {
			tile_set_desktop(t, n);
			tile_hide(t);
			break;
		}
	);
}

void go_to_desktop(int n) {
	if (n == _active_desktop) return;

	_active_desktop = n;
	foreach_tile(_tiles, t,
		if (tile_get_desktop(t) == _active_desktop) {
			tile_show(t);
		} else {
			tile_hide(t);
		}		
	);
}

void layout_init() {	
	_tiles = list_create();
	//TODO: exclude it to separete event source
	EnumWindows(enum_windows_proc, 0);
}

void layout_dispose() {	
	foreach_tile(_tiles, t, tile_show(t));
	log_print("show all windows: Completed");

	foreach_tile(_tiles, t,	tile_dispose(t));
	log_print("dispose each tile: Completed");

	list_dispose(_tiles);
	log_print("dispose tiles list: Completed");
}