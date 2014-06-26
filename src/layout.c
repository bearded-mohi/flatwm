#include <windows.h>
#include "layout.h"

#define MAX_TILES_COUNT 1024

typedef struct Tile {
	HWND hwnd;
	char * className;
	char * title;
	int desktop;
} Tile;

static Tile * _tiles[MAX_TILES_COUNT];
static int _tilesCount = 0;
static int _activeDesktop = 1;

static Tile * createTile(HWND hwnd, const char * className, const char * title, int desktop) {
	Tile * t = malloc(sizeof(Tile));	
	t->hwnd = hwnd;
	t->className = malloc(strlen(className) + 1);
	strcpy(t->className, className);
	t->title = malloc(strlen(title) + 1);	
	strcpy(t->title, title);
	t->desktop = desktop;
	return t;
}

static void disposeTile(Tile * tile) {
	free(tile->className);
	free(tile->title);
	free(tile);
}

static BOOL isWindowManagable(HWND hwnd) {
	TITLEBARINFO ti;
    HWND hwndTry, hwndWalk = NULL;
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
    if(ti.rgstate[0] & STATE_SYSTEM_INVISIBLE) {
        return FALSE;
    }
    if(GetWindowLong(hwnd, GWL_EXSTYLE) & WS_EX_TOOLWINDOW) {
        return FALSE;
    }
    return TRUE;
}

static BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
	char title[256];
	char className[256];
	GetWindowText(hwnd, title, 256);
	GetClassName(hwnd, className, 256);
	if(_tilesCount < MAX_TILES_COUNT - 1) {
		if(isWindowManagable(hwnd)) {
			_tiles[_tilesCount++] = createTile(hwnd, className, title, 1);
		}
		return TRUE;
	} else {
		return FALSE;
	}
}

static void restoreAll() {
	for(int i = 0; i < _tilesCount; i++) {
		ShowWindow(_tiles[i]->hwnd, SW_SHOW);
	}
}


static BOOL _systemAnimations;
static void disableSystemAnimations() {
	ANIMATIONINFO ai;
	ai.cbSize = sizeof(ANIMATIONINFO); 
	SystemParametersInfo(SPI_GETANIMATION, sizeof(ANIMATIONINFO), &ai, 0); 
	_systemAnimations = ai.iMinAnimate; 

	if(_systemAnimations) { 
	ai.iMinAnimate = FALSE; 
		SystemParametersInfo(SPI_SETANIMATION, sizeof(ANIMATIONINFO), &ai, 0); 
	} 	
}

static void restoreSystemAnimations() {
	ANIMATIONINFO ai;
	ai.cbSize = sizeof(ANIMATIONINFO); 
	if(_systemAnimations) { 
	ai.iMinAnimate = TRUE; 
		SystemParametersInfo(SPI_SETANIMATION, sizeof(ANIMATIONINFO), &ai, 0); 
	} 		
}

void initLayout() {
	disableSystemAnimations();
	EnumWindows(EnumWindowsProc, 0);
}

void listLayout() {
	size_t totalLen = 0;	
	for(int i = 0; i < _tilesCount; i++) {
		totalLen += strlen(_tiles[i]->className) + 1;
		totalLen += strlen(_tiles[i]->title) + 1;
	}
	char * totalList = malloc(totalLen + 1);
	memset(totalList, '\0', totalLen);
	for(int i = 0; i < _tilesCount; i++) {
		strcat(totalList, _tiles[i]->className);
		strcat(totalList, "-");
		strcat(totalList, _tiles[i]->title);
		strcat(totalList, "\n");
	}
	MessageBox(NULL, totalList, "tiles list", MB_OK);
}

void moveWindowToDesktop(int n) {
	HWND foreground = GetForegroundWindow();
	for(int i = 0; i < _tilesCount; i++) {
		if(_tiles[i]->hwnd == foreground) {
			_tiles[i]->desktop = n;
			if(_activeDesktop != n) {
				ShowWindow(_tiles[i]->hwnd, SW_HIDE);
			}
			break;
		} 
	}
}

void goToDesktop(int n) {
	_activeDesktop = n;
	for(int i = 0; i < _tilesCount; i++) {
		if(_tiles[i]->desktop == _activeDesktop) {
			ShowWindow(_tiles[i]->hwnd, SW_SHOWNA);
		} else {
			ShowWindow(_tiles[i]->hwnd, SW_HIDE);
		}
	}
}

void disposeLayout() {
	restoreSystemAnimations();
	restoreAll();
	for(int i = 0; i < _tilesCount; i++) {
		disposeTile(_tiles[i]);
	}
	_tilesCount = 0;
}