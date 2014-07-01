#include "tile_list.h"

struct List {
	Tile_Item *head;
	Tile_Item *tail;
};

List *list_create() {
	List *self = malloc(sizeof(List));
	self->head = NULL;
	self->tail = NULL;
	return self;
}

Tile_Item * const list_head(List *self) {
	return self->head;
}

void list_add(List *self, Tile *tile) {
	Tile_Item *new_item = malloc(sizeof(Tile_Item));
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

void list_remove(List *self, HWND hwnd) {
	Tile_Item *cur = self->head;
	Tile_Item *prev = NULL;
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

bool list_has(List *self, HWND hwnd) {
	foreach_tile(self, t,
		if (tile_get_hwnd(t) == hwnd) return true;
	);
	return false;
}

void list_dispose(List *self) {
	Tile_Item *cur = self->head;
	while (NULL != self->head) {				
		cur = self->head;
		self->head = self->head->next;
		free(cur);
	}
	free(self);
}