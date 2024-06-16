#ifndef CHECKER_H
#define CHECKER_H

#include <SDL2/SDL.h>
#include <stdbool.h>

// Const colors
#define COLOR_RED 1
#define COLOR_BLACK 0

// Checker struct
typedef struct {
  // Position
  int x;
  int y;

  // Size
  int width;
  int height;

  // Type
  int type;
  int color;

  // King
  int king;

  // Image
  SDL_Texture* image;
  SDL_Texture* image_king;
} Checker;

// Init
void init_checker(Checker* checker);

// Draw
void draw_checker(Checker* checker);

// At position
bool is_checker_at(Checker* checker, int x, int y);

// Move
void jump_checker(Checker* checker, bool left, bool super, bool backwards);

// Generate image
SDL_Texture* generate_image(int width, int height, int color, bool king);

#endif  // CHECKER_H
