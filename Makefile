# Detect platform
ifeq ($(OS),Windows_NT)
  # Windows (MinGW/MSYS2) - static linking
  CC       ?= gcc
  CFLAGS   ?= -O3 -Wall -Werror -IC:/msys64/mingw64/include/SDL2 -Dmain=SDL_main
  # Link statically against SDL2, TTF, Mixer and system libs
  SDL_LINKER_FLAGS := `pkg-config --libs --static SDL2`
  SDL_LINKER_FLAGS += `pkg-config --libs --static SDL2_ttf`
  SDL_LINKER_FLAGS += `pkg-config --libs --static SDL2_image`
  SDL_LINKER_FLAGS += `pkg-config --libs --static SDL2_Mixer`
  SDL_LINKER_FLAGS += -lbrotlicommon -lsharpyuv -lstdc++ -lgcc -lssp
  LDFLAGS  ?= -static \
               -lm -lmingw32 -lws2_32 \
               -lSDL2main -lSDL2 -lSDL2_TTF -lSDL2_Mixer \
               -LC:/msys64/mingw64/lib \
               $(SDL_LINKER_FLAGS)
  TARGET   := 2048pvp.exe
  # No DLLs needed when static linked
else
  # Unix-like (Linux, macOS)
  CC       ?= clang
  CFLAGS   ?= -O3 -Wall -Werror $(shell sdl2-config --cflags)
  LDFLAGS  ?= -flto $(shell sdl2-config --libs) -lSDL2_ttf -lSDL2_mixer -lSDL2_image
  TARGET   := 2048pvp
endif

SRC_DIR     := src
OBJ_DIR     := obj
INCLUDE_DIR := include

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

run: all
	./$(TARGET)

demo: all
	./$(TARGET) --demo

package: all
ifeq ($(OS),Windows_NT)
	@echo Packaging Windows static portable zip...
	@mkdir -p dist
	@cp -r $(TARGET) assets dist
	@cd dist && zip -r ../2048pvp-windows.zip .
	@echo Created 2048pvp-windows.zip
else
	@echo Not implemented...
endif

clean:
ifeq ($(OS),Windows_NT)
	rm -rf $(OBJ_DIR) $(TARGET) 2048pvp-windows.zip dist
else
	rm -rf $(OBJ_DIR) $(TARGET)
endif

.PHONY: all clean run demo package
