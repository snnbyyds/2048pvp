# 2048PvP

(Class project of program design course in Nanjing University. **Do not copy the code if you are making one as well.**)

`2048PvP` is a two-player competitive variant of the classic 2048 game, built in `C` using `SDL2` for graphics, `SDL2_ttf` for text rendering, and `SDL2_mixer` for audio. Players alternate moves (`W`/`A`/`S`/`D` or arrow keys) to slide and merge tiles on an `8`×`8` grid. The first to create a tile of value `2048` wins; if no moves remain, the player with the **highest** tile wins.

---

## Features

- **Local PvP**: Two players on the same machine share keyboard controls.  
- **Demo Mode**: Auto-play (`--demo` flag).  
- **Animated Menu**: Dynamic gradient background with mouse and keyboard navigation.  
- **Sound Effects & Music**: Tile merge sound and looping background music.
- **Platform Compability**: Tested on both Linux and Windows  

---

## Dependencies

- **SDL2** (>=2.0.1)  
- **SDL2_ttf**  
- **SDL2_mixer**  
- **SDL2_image**
- **clang** or **gcc**

### Arch Linux

```bash
# Install dependencies via pacman
sudo pacman -Syu sdl2 sdl2_ttf sdl2_mixer sdl2_image clang make
```
### Debian / Ubuntu
```bash
# Install dependencies via apt
sudo apt update
sudo apt install -y \
    libsdl2-dev libsdl2-ttf-dev libsdl2-mixer-dev \
    clang make
```

### Windows (MinGW/MSYS2)
```bash
pacman -S --needed \
    mingw-w64-x86_64-toolchain \
    mingw-w64-x86_64-make \
    mingw-w64-x86_64-SDL2 \
    mingw-w64-x86_64-SDL2_ttf \
    mingw-w64-x86_64-SDL2_mixer \
    mingw-w64-x86_64-SDL2_image \
    mingw-w64-x86_64-libsharpyuv \
    zip
```

## Build Instructions

The project uses a simple Makefile. You can customize the compiler and flags via environment variables.

```bash
make         # build
make run     # start PvP mode
make demo    # start demo (AI) mode
make clean   # remove build artifacts
make package # package artifact (Windows only)
```

## Usage

- Start Game: Launch normally or select Start Game in the menu.

- Demo: Pass `--demo` or select `Demo`

- Move: `W`/`A`/`S`/`D` or `↑`/`←`/`↓`/`→`

- Exit: `ESC` or window close

