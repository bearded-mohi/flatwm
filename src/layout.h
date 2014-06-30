#ifndef LAYOUT_H
#define LAYOUT_H

void layout_init();
void layout_list();
void layout_track(HWND hwnd);
void layout_untrack(HWND hwnd);
void go_to_desktop(int n);
void move_window_to_desktop(int n);
void layout_dispose();

#endif