#ifndef TILE_H
#define TILE_H

#include <windows.h>

typedef struct Tile Tile;

Tile              *tile_create(HWND hwnd, const char *class_name, const char *caption, int desktop);
HWND               tile_get_hwnd(Tile *self);
char const * const tile_get_class_name(Tile *self);
char const * const tile_get_caption(Tile *self);
int                tile_get_desktop(Tile *self);
void               tile_set_desktop(Tile *self, int desktop);
void               tile_dispose(Tile *self);

#endif