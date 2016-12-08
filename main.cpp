// Includes
#include <allegro.h>

#include "board.h"

// Buffer
BITMAP *buffer;

// Main board
board main_board;

// Init game
void init(){
  // Init allegro
  allegro_init();
  install_mouse();
  install_keyboard();
  install_timer();

  // Init graphics
  set_color_depth( 32);
  set_gfx_mode( GFX_AUTODETECT_WINDOWED, 480, 480, 0, 0);

  // Create buffer
  buffer = create_bitmap( SCREEN_W, SCREEN_H);

  // Create a board
  main_board = board( 10, 10, SCREEN_H, SCREEN_H);
}

// Run game
void update(){

}

// Draw
void draw(){
  // Clear buffer
  clear_to_color( buffer, 0x000000);

  // Draw board
  main_board.draw( buffer);

  // Draw buffer to screen
  draw_sprite( screen, buffer, 0, 0);
}

// Main
int main(){
  // Setup
  init();

  // Run loops
  while( !key[KEY_ESC]){
    update();
    draw();
  }

  // End
  return 0;
}
END_OF_MAIN();
