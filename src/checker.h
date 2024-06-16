#ifndef CHECKER_H
#define CHECKER_H

// SDL included for drawing
#include <SDL2/SDL.h>

// Const colors
const int COLOR_RED = 1;
const int COLOR_BLACK = 0;

// Checker class
class checker {
 public:
  // Constructors
  checker();
  checker(int x, int y, int width, int height, int color);

  // Draw
  void draw();

  // At position
  bool is_at(int x, int y);

  // Move
  void jump(bool left, bool super, bool backwards);

  // Type
  int type;
  int color;

  // Get x and y
  int get_x() { return x; }
  int get_y() { return y; }

  // King me
  void king_me() { king = true; }

  // King
  bool king;

 protected:
 private:
  // Image
  SDL_Texture* image;
  SDL_Texture* image_king;

  // Position
  int x;
  int y;

  // Size
  int width;
  int height;

  // Generate image
  static SDL_Texture* generate_image(int width,
                                     int height,
                                     int color,
                                     bool king = false);
};

#endif  // CHECKER_H
