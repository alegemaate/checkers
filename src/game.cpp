#include "game.h"

// Init game state
game::game(){
  // Create buffer
  buffer = create_bitmap( SCREEN_W, SCREEN_H);

  // Create a board
  main_board = board( SCREEN_H, SCREEN_H);

  // Make some checkers
  for( int t = 0; t < 3; t += 1){
    for( int i = t % 2; i < TILES_WIDE; i += 2){
      checker newChecker( i * SCREEN_H/TILES_WIDE, SCREEN_H/TILES_WIDE * t, SCREEN_H/TILES_HIGH, SCREEN_H/TILES_HIGH, COLOR_RED);
      main_board.add_checker( newChecker);
    }
  }

  for( int t = TILES_WIDE - 3; t < TILES_WIDE; t += 1){
    for( int i = t % 2; i < TILES_WIDE; i += 2){
      checker newChecker( i * SCREEN_H/TILES_WIDE, SCREEN_H/TILES_WIDE * t, SCREEN_H/TILES_HIGH, SCREEN_H/TILES_HIGH, COLOR_BLACK);
      main_board.add_checker( newChecker);
    }
  }

  // Player 1's turn
  turn = 0;
}

// Destroy game state
game::~game(){
  //dtor
}

// Run game
void game::update(){
  // Select tile
  if( mouse_b & 1){
    main_board.select_tile( mouse_x/(SCREEN_H/TILES_WIDE), mouse_y/(SCREEN_H/TILES_WIDE));
  }
}

// Draw
void game::draw(){
  // Clear buffer
  clear_to_color( buffer, 0x000000);

  // Draw board
  main_board.draw( buffer);

  // Mouse cursor
  circlefill( buffer, mouse_x, mouse_y, 5, 0x000000);
  circlefill( buffer, mouse_x, mouse_y, 3, 0xFFFFFF);

  // Debug
  textprintf_ex( buffer, font, 500, 40, 0xFFFFFF, -1, "Turn:%i", turn);

  // Draw buffer to screen
  draw_sprite( screen, buffer, 0, 0);
}
