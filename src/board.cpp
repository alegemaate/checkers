#include "board.h"

// Init the board (no parameters)
board::board(){
  // Size
  this -> width = 1;
  this -> height = 1;

  // Selection
  selection_x = -1;
  selection_y = -1;

  // Null point image
  image_board = NULL;

  // Multimove activated
  multimove = false;
}

// Init the board
board::board( int width, int height){
  // Size
  this -> width = width;
  this -> height = height;

  // Selection
  selection_x = -1;
  selection_y = -1;

  // Null point image
  image_board = NULL;

  // Make the board
  image_board = generate_board( this -> width, this -> height);

  // Multimove activated
  multimove = false;
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

  // Available moves
  for( unsigned int i = 0; i < moves.size(); i++)
    rectfill( tempBuffer, moves.at(i).x * width/TILES_WIDE, moves.at(i).y * height/TILES_HIGH, (moves.at(i).x + 1) * width/TILES_WIDE - 1, (moves.at(i).y + 1) * height/TILES_HIGH - 1, 0x00FF00);

  // Draw pieces
  for( unsigned int i = 0; i < checkers.size(); i++)
    checkers.at(i).draw( tempBuffer);

  // Debug
  textprintf_ex( tempBuffer, font, 500, 20, 0xFFFFFF, -1, "Moves:%i", moves.size());
}

// Add checkers
void board::add_checker( checker newChecker){
  checkers.push_back( newChecker);
}

// Calculate moves for selected checker
void board::calculate_moves( position selectedTile, int color, bool king, bool first){
  // Left side and then right
  for( int i = -1; i <= 1; i += 2){
    // Backward for kings
    for( int t = 0; t <= king; t += 1){
      // Tile to check
      int new_x = selectedTile.x + i;
      int new_y = selectedTile.y + ((color * 2) - 1) * -((t * 2) - 1);

      // Single movements
      if( checker_at( position(new_x, new_y)) == -1 && first){
        moves.push_back( position( new_x, new_y));
      }

      // Jumps ( color is opposite, space behind to land)
      else if( checker_at( position(new_x, new_y)) >= 0 && checkers.at(checker_at( position(new_x, new_y))).color != color && checker_at( position(new_x + i, new_y + ((color * 2) - 1) * -((t * 2) - 1))) == -1){
        moves.push_back( position( new_x + i, new_y + ((color * 2) -1) * -((t * 2) - 1)));
        checker_jumped.x = new_x;
        checker_jumped.y = new_y;
      }
    }
  }
}

// Checker at pos
int board::checker_at( position newPosition){
  // Out of range
  if( newPosition.x < 0 || newPosition.y < 0 || newPosition.x >= TILES_WIDE || newPosition.y >= TILES_HIGH)
    return -2;
  // Check
  for( unsigned int i = 0; i < checkers.size(); i++){
    if( checkers.at(i).is_at( newPosition.x, newPosition.y)){
      return i;
      break;
    }
  }
  return -1;
}

// Move at position
int board::move_at( position newPosition){
  // Out of range
  if( newPosition.x < 0 || newPosition.y < 0 || newPosition.x >= TILES_WIDE || newPosition.y >= TILES_HIGH)
    return -2;
  // Check
  for( unsigned int i = 0; i < moves.size(); i++){
    if( moves.at(i).x == newPosition.x && moves.at(i).y == newPosition.y){
      return i;
      break;
    }
  }
  return -1;
}

// Select tile
void board::select_tile( position newPosition){
  // Check if it is a move
  if( move_at( position( newPosition.x, newPosition.y)) >= 0){
    // Get index of selected checker
    int checker_to_move = checker_at( position(selection_x, selection_y));
    bool double_jump = (abs(selection_y - newPosition.y) > 1);
    checkers.at( checker_to_move).jump( selection_x > newPosition.x, double_jump, ((checkers.at(checker_to_move).color == 0) && (selection_y < newPosition.y)) || ((checkers.at(checker_to_move).color == 1) && (selection_y > newPosition.y)));

    // Check if needs kinging
    if( checkers.at( checker_to_move).get_y() == (TILES_HIGH - 1) || checkers.at( checker_to_move).get_y() == 0){
      checkers.at( checker_to_move).king_me();
    }

    // Erase jumped enemy
    if( double_jump){
      // Remove jumped checker
      checkers.erase( checkers.begin() + checker_at(position(checker_jumped.x, checker_jumped.y)));
      // Clear all moves (needs recalculating)
      moves.clear();
      // Recalculate moves
      calculate_moves( position(checkers.at( checker_to_move).get_x(), checkers.at( checker_to_move).get_y()), checkers.at( checker_to_move).color, checkers.at( checker_to_move).king, false);
      // No more moves
      if( moves.size() == 0){
        globals::turn = !globals::turn;
        multimove = false;
      }
      else{
        multimove = true;
      }
    }
    else{
      // Change turn
      globals::turn = !globals::turn;
      moves.clear();
      multimove = false;
    }
  }
  else{
    // Clear moves
    moves.clear();

    // Piece Exists
    selection_x = newPosition.x;
    selection_y = newPosition.y;
    int selection_index = checker_at( position(selection_x, selection_y));
    if( selection_index <= -1 || checkers.at(selection_index).color != globals::turn){
      selection_x = -1;
      selection_y = -1;
    }
    else if( !multimove){
      // Calc new moves
      calculate_moves( position( selection_x, selection_y), checkers.at(selection_index).color, checkers.at(selection_index).king, true);
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
