#include <stdio.h>
#include <stdbool.h>
#include <windows.h>

#include "log.h"
#include "tile.h"
#include "layout.h"

#define foreach_tile(t, ...) { \
	tile_item_s *it = _tiles->head; \
	while (NULL != it) { \
		Tile *t = it->item; \
		__VA_ARGS__; \
		it = it->next; \
	} \
}

static BOOL is_window_managable(HWND hwnd);

typedef struct tile_item_s {
	Tile               *item;
	struct tile_item_s *next;
} tile_item_s;

typedef struct tile_list_s {
	tile_item_s *head;
	tile_item_s *tail;
} tile_list_s;

tile_list_s *list_create() {
	tile_list_s *self = malloc(sizeof(tile_list_s));
	self->head = NULL;
	self->tail = NULL;
	return self;
}

void list_dispose(tile_list_s *self) {
	tile_item_s *cur = self->head;
	while (NULL != self->head) {				
		cur = self->head;
		self->head = self->head->next;
		free(cur);
	}
	free(self);
}

void list_add(tile_list_s *self, Tile *tile) {
	tile_item_s *new_item = malloc(sizeof(tile_item_s));
	new_item->next = NULL;
	new_item->item = tile;
	if (NULL == self->head) {
		self->head = new_item;
		self->tail = new_item;
	} else {
		self->tail->next = new_item;
		self->tail = new_item;
	}		
}

void list_remove(tile_list_s *self, HWND hwnd) {
	tile_item_s *cur = self->head;
	tile_item_s *prev = NULL;
	while (NULL != cur) {
		if (tile_get_hwnd(cur->item) == hwnd) {
			if (NULL == prev) {
				self->head = cur->next;
				tile_dispose(cur->item);
			} else {
				prev->next = cur->next;
				if (self->tail == cur) {
					self->tail = prev;
				}
				tile_dispose(cur->item);
			}
		}
		prev = cur;
		cur = cur->next;
	}
}

static tile_list_s *_tiles;

static int _active_desktop = 1;

static bool has_tile(HWND hwnd) {
	foreach_tile(t,
		if (tile_get_hwnd(t) == hwnd) return true;
	);
	return false;
}

void layout_track(HWND hwnd) {			
	if (!has_tile(hwnd)) {
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

static void show_all_windows() {
	foreach_tile(t,
		ShowWindow(tile_get_hwnd(t), SW_SHOW);
	)
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
	foreach_tile(t,
		total_len += strlen(tile_get_class_name(t)) + 1;
		total_len += strlen(tile_get_caption(t)) + 1;
	);
	
	char *total_list = malloc(total_len + 1);
	int pos = 0;
	int res = 0;
	foreach_tile(t,
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
	foreach_tile(t,
		if (tile_get_hwnd(t) == foreground) {
			tile_set_desktop(t, n);
			ShowWindow(tile_get_hwnd(t), SW_HIDE);
			break;
		}
	);
}

void go_to_desktop(int n) {
	if (n == _active_desktop) return;

	_active_desktop = n;
	foreach_tile(t,
		ShowWindow(tile_get_hwnd(t), (tile_get_desktop(t) == _active_desktop) ? SW_SHOWNA : SW_HIDE);
	);
}

void layout_init() {	
	_tiles = list_create();
	//TODO: exclude it to separete event source
	EnumWindows(enum_windows_proc, 0);
}

void layout_dispose() {	
	show_all_windows();
	log_print("show_all_windows: Completed");
	foreach_tile(t,	tile_dispose(t));
	log_print("dispose each tile: Completed");
	list_dispose(_tiles);
	log_print("dispose tiles list: Completed");
}