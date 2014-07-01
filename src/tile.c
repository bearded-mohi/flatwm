#include <stdlib.h>
#include <string.h>

#include "tile.h"
#include "log.h"

struct Tile {
	HWND  hwnd;
	char *class_name;
	char *caption;
	int   desktop;
};

Tile *tile_create(HWND hwnd, char const *class_name, char const *caption, int desktop) {
	Tile *self = malloc(sizeof(Tile));	
	self->hwnd = hwnd;
	self->class_name = malloc(strlen(class_name) + 1);
	strcpy(self->class_name, class_name);
	self->caption = malloc(strlen(caption) + 1);	
	strcpy(self->caption, caption);
	self->desktop = desktop;
	log_print("+ tile_create:\n\tclass: %s\n\tcaption: %s\n", self->class_name, self->caption);
	return self;
}

HWND tile_get_hwnd(Tile *self) {
	return self->hwnd;
}

char const * const tile_get_class_name(Tile *self) {
	return self->class_name;
}

char const * const tile_get_caption(Tile *self) {
	return self->caption;
}

int tile_get_desktop(Tile *self) {
	return self->desktop;
}

void tile_set_desktop(Tile *self, int desktop) {
	self->desktop = desktop;
}

void tile_dispose(Tile *self) {
	log_print("- tile_dispose:\n\tclass: %s\n\tcaption: %s\n", self->class_name, self->caption);
	free(self->class_name);
	free(self->caption);
	free(self);
}