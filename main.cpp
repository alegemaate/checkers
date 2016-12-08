// Includes
#include <allegro.h>

// Game state
#include "game.h"

// Make a game
game *checkers_game;

// Init game
void init(){
  // Init allegro
  allegro_init();
  install_mouse();
  install_keyboard();
  install_timer();

  // Init graphics
  set_color_depth( 32);
  set_gfx_mode( GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0);

  // Create game
  checkers_game = new game;
}

// Main
int main(){
  // Setup
  init();

  // Run loops
  while( !key[KEY_ESC]){
    checkers_game -> update();
    checkers_game -> draw();
  }

  // End
  return 0;
}
END_OF_MAIN();
