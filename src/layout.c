#include <windows.h>
#include <assert.h>

#include "log.h"
#include "layout.h"
#include "hook/hook.h"

#define MAX_TILES_COUNT 1024

static BOOL is_window_managable(HWND hwnd);

typedef struct Tile {
	HWND hwnd;
	char *class_name;
	char *caption;
	int desktop;
} Tile;

static Tile *_tiles[MAX_TILES_COUNT];
static int _tiles_count = 0;
static int _active_desktop = 1;

static Tile* create_tile(HWND hwnd, const char *class_name, const char *caption, int desktop) {
	Tile *t = malloc(sizeof(Tile));	
	t->hwnd = hwnd;
	t->class_name = malloc(strlen(class_name) + 1);
	strcpy(t->class_name, class_name);
	t->caption = malloc(strlen(caption) + 1);	
	strcpy(t->caption, caption);
	t->desktop = desktop;
	return t;
}

static void dispose_tile(Tile *tile) {
	free(tile->class_name);
	free(tile->caption);
	free(tile);
}

void insert_tile(HWND hwnd) {		
	if (_tiles_count < MAX_TILES_COUNT - 1) {
		if (is_window_managable(hwnd)) {
			char caption[256];
			char class_name[256];
			GetWindowText(hwnd, caption, 256);
			GetClassName(hwnd, class_name, 256);			
			_tiles[_tiles_count++] = create_tile(hwnd, class_name, caption, _active_desktop);
		}		
	}	
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
    // if(ti.rgstate[0] & STATE_SYSTEM_INVISIBLE) {
    //     return FALSE;
    // }
    if(GetWindowLong(hwnd, GWL_EXSTYLE) & WS_EX_TOOLWINDOW) {
        return FALSE;
    }
    return TRUE;
}

static void show_all_windows() {
	for (int i = 0; i < _tiles_count; i++) {
		ShowWindow(_tiles[i]->hwnd, SW_SHOW);
	}
}


static BOOL _system_animations;
static void disable_system_animations() {
	ANIMATIONINFO ai;
	ai.cbSize = sizeof(ANIMATIONINFO); 
	SystemParametersInfo(SPI_GETANIMATION, sizeof(ANIMATIONINFO), &ai, 0); 
	_system_animations = ai.iMinAnimate; 

	if (_system_animations) { 
		ai.iMinAnimate = FALSE; 
		SystemParametersInfo(SPI_SETANIMATION, sizeof(ANIMATIONINFO), &ai, 0); 
	} 	
}

static void restore_system_animations() {
	ANIMATIONINFO ai;
	ai.cbSize = sizeof(ANIMATIONINFO); 
	if (_system_animations) { 
		ai.iMinAnimate = TRUE; 
		SystemParametersInfo(SPI_SETANIMATION, sizeof(ANIMATIONINFO), &ai, 0); 
	} 		
}

static BOOL CALLBACK enum_windows_proc(HWND hwnd, LPARAM lParam) {
	insert_tile(hwnd);
	return TRUE;
}

void init_layout() {
	// registerWindowHooks(insertTile, onDestroyWindow);
	disable_system_animations();
	EnumWindows(enum_windows_proc, 0);
}

void list_layout() {
	size_t total_len = 0;	
	for (int i = 0; i < _tiles_count; i++) {
		total_len += strlen(_tiles[i]->class_name) + 1;
		total_len += strlen(_tiles[i]->caption) + 1;
	}
	char *total_list = malloc(total_len + 1);
	memset(total_list, '\0', total_len);
	for (int i = 0; i < _tiles_count; i++) {
		strcat(total_list, _tiles[i]->class_name);
		strcat(total_list, "-");
		strcat(total_list, _tiles[i]->caption);
		strcat(total_list, "\n");
	}
	MessageBox(NULL, total_list, "tiles list", MB_OK);
	free(total_list);
}

void move_window_to_desktop(int n) {
	if (n == _active_desktop) return;

	HWND foreground = GetForegroundWindow();
	for (int i = 0; i < _tiles_count; i++) {
		if (_tiles[i]->hwnd == foreground) {			
			_tiles[i]->desktop = n;
			ShowWindow(_tiles[i]->hwnd, SW_HIDE);
			break;		
		} 
	}
}

void go_to_desktop(int n) {
	if (n == _active_desktop) return;

	_active_desktop = n;
	for (int i = 0; i < _tiles_count; i++) {
		if (_tiles[i]->desktop == _active_desktop) {
			ShowWindow(_tiles[i]->hwnd, SW_SHOWNA);
		} else {
			ShowWindow(_tiles[i]->hwnd, SW_HIDE);
		}
	}
}

void dispose_layout() {
	// unregisterWindowHooks();
	restore_system_animations();
	show_all_windows();
	for (int i = 0; i < _tiles_count; i++) {
		dispose_tile(_tiles[i]);
	}
	_tiles_count = 0;
}