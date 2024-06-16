#include "game.h"
#include "core.h"
#include "globals.h"

// Init game state
game::game() {
  // Create a board
  main_board = board(480, 480);

  // Make some checkers
  for (int t = 0; t < 3; t += 1) {
    for (int i = t % 2; i < TILES_WIDE; i += 2) {
      checker newChecker(i, t, 480 / TILES_HIGH, 480 / TILES_HIGH, COLOR_RED);
      main_board.add_checker(newChecker);
    }
  }

  for (int t = TILES_WIDE - 3; t < TILES_WIDE; t += 1) {
    for (int i = t % 2; i < TILES_WIDE; i += 2) {
      checker newChecker(i, t, 480 / TILES_HIGH, 480 / TILES_HIGH, COLOR_BLACK);
      main_board.add_checker(newChecker);
    }
  }
}

// Run game
void game::update() {
  int mouse_x, mouse_y;
  auto mouse_b = SDL_GetMouseState(&mouse_x, &mouse_y);

  // Select tile
  if (mouse_b & SDL_BUTTON(SDL_BUTTON_LEFT)) {
    main_board.select_tile(
        position(mouse_x / (480 / TILES_WIDE), mouse_y / (480 / TILES_WIDE)));
  }
  if (mouse_b & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
    checker newChecker(mouse_x / (480 / TILES_WIDE),
                       mouse_y / (480 / TILES_WIDE), 480 / TILES_HIGH,
                       480 / TILES_HIGH, COLOR_BLACK);
    main_board.add_checker(newChecker);
  }
}

// Draw
void game::draw() {
  int mouse_x, mouse_y;
  SDL_GetMouseState(&mouse_x, &mouse_y);

  // Clear buffer
  // clear_to_color(buffer, 0x000000);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  // Draw board
  main_board.draw();

  // Mouse cursor
  // circlefill(buffer, mouse_x, mouse_y, 5, 0x000000);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_Rect rect1 = {mouse_x, mouse_y, 3, 3};
  SDL_RenderFillRect(renderer, &rect1);

  // circlefill(buffer, mouse_x, mouse_y, 3, 0xFFFFFF);
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_Rect rect2 = {mouse_x, mouse_y, 3, 3};
  SDL_RenderFillRect(renderer, &rect2);

  // Debug
  // textprintf_ex(buffer, font, 500, 40, 0xFFFFFF, -1, "Turn:%i",
  // globals::turn);

  // Draw buffer to screen
  // draw_sprite(screen, buffer, 0, 0);

  SDL_RenderPresent(renderer);
}
