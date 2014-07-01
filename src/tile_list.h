#ifndef TILE_LIST_H
#define TILE_LIST_H

#include <stdbool.h>
#include <windows.h>

#include "tile.h"

//TODO: create iterator and hide this struct
typedef struct Tile_Item {
	Tile               *item;
	struct Tile_Item   *next;
} Tile_Item;

typedef struct List List;

List             *list_create();
Tile_Item * const list_head(List *self);
void              list_add(List *self, Tile *tile);
void              list_remove(List *self, HWND hwnd);
bool              list_has(List *self, HWND hwnd);
void              list_dispose(List *self);

#define foreach_tile(list, t, ...) { \
	Tile_Item *ti = list_head(list); \
	while (NULL != ti) { \
		Tile *t = ti->item; \
		__VA_ARGS__; \
		ti = ti->next; \
	} \
}

#endif