#ifndef GAME_H
#define GAME_H

// Includes
#include <SDL2/SDL.h>

// Board
#include "board.h"

// Checkers
#include "checker.h"

// Global vars
#include "globals.h"

// Main game
class game {
 public:
  game();

  // Update
  void update();

  // Drawing
  void draw();

 private:
  // Main board
  board main_board;
};

#endif  // GAME_H
