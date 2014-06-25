#include <windows.h>
#include "config.h"

int CALLBACK WinMain(
  HINSTANCE hInstance,
  HINSTANCE hPrevInstance,
  LPSTR lpCmdLine,
  int nCmdShow
) {
	applyConfig("_flatwmrc");
	unregisterHotkeys();	
	return 0;
}