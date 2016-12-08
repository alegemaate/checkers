#include "board.h"

// Init the board (no parameters)
board::board(){
  // Size
  this -> width = 1;
  this -> height = 1;

  // Selection
  selection_x = 0;
  selection_y = 0;

  // Null point image
  image_board = NULL;
}

// Init the board
board::board( int width, int height){
  // Size
  this -> width = width;
  this -> height = height;

  // Selection
  selection_x = 0;
  selection_y = 0;

  // Null point image
  image_board = NULL;

  // Make the board
  image_board = generate_board( this -> width, this -> height);
}

// Destroy the board
board::~board(){
  //dtor
}

// Draw the board
void board::draw( BITMAP *tempBuffer){
  // IF it exists
  if( image_board != NULL)
    draw_sprite( tempBuffer, image_board, 0, 0);

  // Draw selection
  rectfill( tempBuffer, selection_x * width/TILES_WIDE, selection_y * height/TILES_HIGH, (selection_x + 1) * width/TILES_WIDE - 1, (selection_y + 1) * height/TILES_HIGH - 1, 0xFFFF00);

  // Draw pieces
  for( unsigned int i = 0; i < checkers.size(); i++)
    checkers.at(i).draw( tempBuffer);

  // Debug
  textprintf_ex( tempBuffer, font, 500, 20, 0xFFFFFF, -1, "Selection X/Y:%i %i", selection_x, selection_y);
}

// Add checkers
void board::add_checker( checker newChecker){
  checkers.push_back( newChecker);
}

// Checker at pos
int board::checker_at( int x, int y){
  for( unsigned int i = 0; i < checkers.size(); i++){
    if( checkers.at(i).is_at( x, y)){
      return i;
      break;
    }
  }
  return -1;
}

// Select tile
void board::select_tile( int x, int y){
  if( x < TILES_HIGH && y < TILES_WIDE ){
    selection_x = x;
    selection_y = y;

    // Jump pieces
    bool superJump = false;
    int selection_index = checker_at( selection_x * (height/TILES_HIGH), selection_y * (width/TILES_WIDE));
    if( selection_index != -1){
      int checker_index2 = checker_at( (selection_x - 1) * (width/TILES_WIDE), (selection_y + ((checkers.at(checker_index).color * 2) - 1)) * (height/TILES_HIGH));
      superJump = (checker_index2 != -1);
      checkers.at(selection_index).jump( true, superJump);
    }
  }
}

// Generate board image
BITMAP *board::generate_board( int width, int height){
  BITMAP *temp_image = create_bitmap( width, height);
  clear_to_color( temp_image, 0x8C5242);

  // Populate squares
  for( int i = 0; i < TILES_WIDE; i += 1){
    for( int t = 0; t < TILES_HIGH; t += 1){
      // If it should be drawn
      if( (i % 2 == 0 && t % 2 == 1) || (i % 2 == 1 && t % 2 == 0))
        rectfill( temp_image, i * width/TILES_WIDE, t * height/TILES_HIGH, (i + 1) * width/TILES_WIDE - 1, (t + 1) * height/TILES_HIGH - 1, 0xFFFFCE);
    }
  }

  // Return the image
  return temp_image;
}
