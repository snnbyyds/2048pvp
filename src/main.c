#include <SDL2/SDL_main.h>
#include <game.h>
#include <board.h>

int main() {
    init_board();
    return rungame();
}
