#ifndef BOARD_H
#define BOARD_H

#include <SDL2/SDL.h>
#include <stdbool.h>

// Checkers type
#include "checker.h"

// Position
#include "position.h"

// Tiles wide and high
#define TILES_WIDE 8
#define TILES_HIGH 8

// Holder of board, contains spaces for checkers
typedef struct {
  int width;
  int height;

  // Stores checkers
  Checker** checkers;

  // Selected coordinates
  int selection_x;
  int selection_y;

  // Multi move
  int multimove;

  Position checker_jumped;

  // Stores available moves
  Position** moves;

  // Img
  SDL_Texture* image_board;
} Board;

// Board functions
void init_board(Board* board, int width, int height);
void draw_board(Board* board);
void add_checker(Board* board, Checker* newChecker);
void calculate_moves(Board* board,
                     Position selectedTile,
                     int color,
                     bool king,
                     bool first);
void select_tile(Board* board, Position newPosition);
SDL_Texture* generate_board(SDL_Renderer* renderer, int width, int height);
int checker_at(Board* board, Position newPosition);

#endif  // BOARD_H
