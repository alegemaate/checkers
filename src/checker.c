#include "checker.h"

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "core.h"

// Constructor with vars
void init_checker(Checker* checker) {
  // Make checker
  checker->image =
      generate_image(checker->width, checker->height, checker->color, false);
  checker->image_king =
      generate_image(checker->width, checker->height, checker->color, true);
}

// Draw
void draw_checker(Checker* checker) {
  // IF it exists
  if (checker->king && checker->image_king != NULL) {
    SDL_Point size;
    SDL_QueryTexture(checker->image_king, NULL, NULL, &size.x, &size.y);
    SDL_Rect dest = {checker->x * checker->width, checker->y * checker->height,
                     checker->width, checker->height};
    SDL_RenderCopy(renderer, checker->image_king, NULL, &dest);

    // draw_sprite(tempBuffer, image_king, x * width, y * height);
  } else if (checker->image != NULL) {
    SDL_Point size;
    SDL_QueryTexture(checker->image, NULL, NULL, &size.x, &size.y);
    SDL_Rect dest = {checker->x * checker->width, checker->y * checker->height,
                     checker->width, checker->height};
    SDL_RenderCopy(renderer, checker->image, NULL, &dest);
    // draw_sprite(tempBuffer, image, x * width, y * height);
  }
}

// At position
bool is_checker_at(Checker* checker, int x, int y) {
  return checker->x == x && checker->y == y;
}

// Move
void jump_checker(Checker* checker, bool left, bool super, bool backwards) {
  checker->y +=
      ((checker->color * 2) - 1) * -((backwards * 2) - 1) * (super + 1);
  if (!left) {
    checker->x += super + 1;
  } else {
    checker->x -= super + 1;
  }
}

// Generate a image
SDL_Texture* generate_image(int width, int height, int color, bool king) {
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
    int center_x = width / 2;
    int center_y = height / 2;
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
