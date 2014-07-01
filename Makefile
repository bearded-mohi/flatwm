P=flatwm.exe
CFG=_flatwmrc
CC=gcc
CFLAGS = -std=c11 -Wall -g -mwindows
BUILD_DIR=build

ifeq ($(PROCESSOR_ARCHITECTURE),x86)
	CFLAGS += -m32
endif
ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
	CFLAGS += -m64
endif

all: exe

build_dir:
	if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
	copy $(CFG) $(BUILD_DIR)

hook_dll: build_dir
	$(CC) -c src/hook/hook.c -o $(BUILD_DIR)/hook.o
	$(CC) -shared -o $(BUILD_DIR)/hook.dll $(BUILD_DIR)/hook.o

exe: hook_dll
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$(P) src/*.c -L./$(BUILD_DIR) -lhook

clean:
	del /s/q build\*
	rmdir build

	
