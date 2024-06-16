#ifndef GAME_H
#define GAME_H

// Includes
#include <SDL2/SDL.h>
#include <stdbool.h>

// Board
#include "board.h"

// Checkers
#include "checker.h"

typedef struct {
  Board main_board;
  bool mouse_down;
} Game;

void init_game(Game* game);
void update_game(Game* game);
void draw_game(Game* game);

#endif  // GAME_H
