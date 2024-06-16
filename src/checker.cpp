#include "checker.h"

#include <SDL2/SDL.h>
#include "core.h"

// Constructor
checker::checker() {
  // Default vars
  this->x = 0;
  this->y = 0;
  this->width = 1;
  this->height = 1;
  this->color = COLOR_RED;
  this->image = NULL;
  this->image_king = NULL;
  this->king = false;
}

// Constructor with vars
checker::checker(int x, int y, int width, int height, int color) {
  // Init vars
  this->x = x;
  this->y = y;
  this->width = width;
  this->height = height;
  this->color = color;
  this->image = NULL;
  this->image_king = NULL;
  this->king = false;

  // Make checker
  this->image = generate_image(this->width, this->height, this->color);
  this->image_king =
      generate_image(this->width, this->height, this->color, true);
}

// Draw
void checker::draw() {
  // IF it exists
  if (king && image_king != NULL) {
    SDL_Point size;
    SDL_QueryTexture(image_king, nullptr, nullptr, &size.x, &size.y);
    SDL_Rect dest = {x * width, y * height, width, height};
    SDL_RenderCopy(renderer, image_king, nullptr, &dest);

    // draw_sprite(tempBuffer, image_king, x * width, y * height);
  } else if (image != NULL) {
    SDL_Point size;
    SDL_QueryTexture(image, nullptr, nullptr, &size.x, &size.y);
    SDL_Rect dest = {x * width, y * height, width, height};
    SDL_RenderCopy(renderer, image, nullptr, &dest);
    // draw_sprite(tempBuffer, image, x * width, y * height);
  }
}

// At position
bool checker::is_at(int x, int y) {
  if (this->x == x && this->y == y) {
    return true;
  }
  return false;
}

// Move
void checker::jump(bool left, bool super, bool backwards) {
  y += ((color * 2) - 1) * -((backwards * 2) - 1) * (super + 1);
  if (!left)
    x += super + 1;
  else
    x -= super + 1;
}

// Generate a image
SDL_Texture* checker::generate_image(int width,
                                     int height,
                                     int color,
                                     bool king) {
  // Make it
  SDL_Texture* texture =
      SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                        SDL_TEXTUREACCESS_TARGET, width, height);

  // Set target
  SDL_SetRenderTarget(renderer, texture);

  // Clear it to transparent
  SDL_RenderClear(renderer);

  // Circle
  SDL_SetRenderDrawColor(renderer, (230 * ((color + 1) % 1)), 0, 0, 255);

  for (int i = 0; i < 360; i++) {
    auto center_x = width / 2;
    auto center_y = height / 2;
    SDL_RenderDrawLine(renderer, center_x, center_y, center_x + (6) * cos(i),
                       center_y + (6) * sin(i));
  }

  SDL_SetRenderDrawColor(renderer, (230 * color), 20, 20, 255);

  for (int i = 0; i < 360; i++) {
    SDL_RenderDrawLine(renderer, width / 2, height / 2,
                       width / 2 + (width / 2 - 8) * cos(i),
                       height / 2 + (height / 2 - 8) * sin(i));
  }

  // King
  if (king) {
    SDL_SetRenderDrawColor(renderer, 128, 128, 20, 255);

    for (int i = 0; i < 360; i++) {
      SDL_RenderDrawLine(renderer, width / 2, height / 2,
                         width / 2 + (width / 3) * cos(i),
                         height / 2 + (height / 3) * sin(i));
    }
  }

  // Clear target
  SDL_SetRenderTarget(renderer, NULL);

  // Return that image
  return texture;
}
