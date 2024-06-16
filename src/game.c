#include "game.h"

#include "core.h"
#include "position.h"

// Init game state
void init_game(Game* game) {
  // Mouse down
  game->mouse_down = false;

  // Create a board
  init_board(&game->main_board, 480, 480);

  // Make some checkers
  for (int t = 0; t < 3; t += 1) {
    for (int i = t % 2; i < TILES_WIDE; i += 2) {
      Checker* newChecker = (Checker*)malloc(sizeof(Checker));
      newChecker->x = i;
      newChecker->y = t;
      newChecker->width = 480 / TILES_HIGH;
      newChecker->height = 480 / TILES_HIGH;
      newChecker->color = COLOR_RED;
      newChecker->king = false;
      init_checker(newChecker);
      add_checker(&game->main_board, newChecker);
    }
  }

  for (int t = TILES_WIDE - 3; t < TILES_WIDE; t += 1) {
    for (int i = t % 2; i < TILES_WIDE; i += 2) {
      Checker* newChecker = (Checker*)malloc(sizeof(Checker));
      newChecker->x = i;
      newChecker->y = t;
      newChecker->width = 480 / TILES_HIGH;
      newChecker->height = 480 / TILES_HIGH;
      newChecker->color = COLOR_BLACK;
      newChecker->king = false;
      init_checker(newChecker);
      add_checker(&game->main_board, newChecker);
    }
  }
}

// Run game
void update_game(Game* game) {
  int mouse_x, mouse_y;
  Uint32 mouse_b = SDL_GetMouseState(&mouse_x, &mouse_y);

  // Select tile
  if (mouse_b & SDL_BUTTON(SDL_BUTTON_LEFT) && !game->mouse_down) {
    select_tile(&game->main_board,
                (Position){.x = mouse_x / (480 / TILES_WIDE),
                           .y = mouse_y / (480 / TILES_WIDE)});
  }
  if (mouse_b & SDL_BUTTON(SDL_BUTTON_RIGHT) && !game->mouse_down) {
    Checker* newChecker = (Checker*)malloc(sizeof(Checker));
    newChecker->x = mouse_x / (480 / TILES_WIDE);
    newChecker->y = mouse_y / (480 / TILES_WIDE);
    newChecker->width = 480 / TILES_HIGH;
    newChecker->height = 480 / TILES_HIGH;
    newChecker->color = COLOR_BLACK;
    newChecker->king = false;
    init_checker(newChecker);
    add_checker(&game->main_board, newChecker);
  }

  game->mouse_down = mouse_b & SDL_BUTTON(SDL_BUTTON_LEFT) ||
                     mouse_b & SDL_BUTTON(SDL_BUTTON_RIGHT);
}

// Draw
void draw_game(Game* game) {
  int mouse_x, mouse_y;
  SDL_GetMouseState(&mouse_x, &mouse_y);

  // Clear buffer
  // clear_to_color(buffer, 0x000000);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  // Draw board
  draw_board(&game->main_board);

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
