// Includes
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

// Game state
#include "core.h"
#include "game.h"

// Make a game
game* checkers_game;

void abortOnError(const std::string& message) {
  SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", message.c_str(),
                           nullptr);
  exit(-1);
}

// Init game
void init() {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
    abortOnError("SDL_Init");
  }

  // Initialize PNG loading
  int imgFlags = IMG_INIT_PNG;
  if (!(IMG_Init(imgFlags) & imgFlags)) {
    abortOnError("IMG_Init");
  }

  if (TTF_Init()) {
    abortOnError("TTF_Init");
  }

  // Initialize SDL_mixer
  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
    abortOnError("Mix_OpenAudio");
  }

  auto window =
      SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                       640, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

  if (!window) {
    abortOnError("WINDOW");
  }

  // Get window surface
  renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  SDL_RenderSetLogicalSize(renderer, 640, 480);

  // Create game
  checkers_game = new game;
}

bool exit_pls = false;

// Main
int main() {
  // Setup
  init();

  // Run loops
  while (!exit_pls) {
    checkers_game->update();
    checkers_game->draw();

    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      switch (e.type) {
        case SDL_QUIT:
          exit_pls = true;
          break;

        default:
          break;
      }
    }
  }

  // End
  return 0;
}
