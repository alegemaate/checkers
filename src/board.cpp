#include "board.h"

// Init the board (no parameters)
board::board(){
  // Size
  this -> width = 1;
  this -> height = 1;

  // Spacing of tiles
  this -> tiles_wide = 1;
  this -> tiles_high = 1;

  // Null point image
  image_board = NULL;
}

// Init the board
board::board( int tiles_wide, int tiles_high, int width, int height){
  // Size
  this -> width = width;
  this -> height = height;

  // Spacing of tiles
  this -> tiles_wide = tiles_wide;
  this -> tiles_high = tiles_high;

  // Null point image
  image_board = NULL;

  // Make the board
  generate_board();
}

// Destroy the board
board::~board(){
  //dtor
}

// Draw the board
void board::draw( BITMAP *tempBuffer){
  // IF it exists
  if( image_board != NULL){
    draw_sprite( tempBuffer, image_board, 0, 0);
  }
}

// Generate board image
void board::generate_board(){
  image_board = create_bitmap( width, height);
  clear_to_color( image_board, 0x000000);

  // Populate squares
  for( int i = 0; i < tiles_wide; i += 1){
    for( int t = 0; t < tiles_high; t += 1){
      // If it should be drawn
      if( (i % 2 == 0 && t % 2 == 1) || (i % 2 == 1 && t % 2 == 0))
        rectfill( image_board, i * width/tiles_wide, t * height/tiles_wide, (i + 1) * width/tiles_wide, (t + 1) * height/tiles_high, 0xFF0000);
    }
  }
}
