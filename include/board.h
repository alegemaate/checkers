#ifndef BOARD_H
#define BOARD_H

// Include allegro for graphics
#include <allegro.h>

// Vector to hold checkers
#include <vector>

// Checkers type
#include "checker.h"

// Position
#include "position.h"

// Tiles wide and high
#define TILES_WIDE 8
#define TILES_HIGH 8

// Holder of board, contains spaces for checkers
class board{
  public:
    board();
    board( int width, int height);
    virtual ~board();

    // Size
    int width;
    int height;

    // Draw
    void draw( BITMAP *tempBuffer);

    // Add checker to board
    void add_checker( checker newChecker);

    // Calculate moves
    void calculate_moves( position selectedTile, int color, bool first = true);

    // Check if checker is at position
    void select_tile( position newPosition);
  protected:

  private:
    // Board generator
    static BITMAP *generate_board( int width, int height);

    // Checker at pos
    int checker_at( position newPosition);

    // Move at position
    int move_at( position newPosition);

    // Image of board
    BITMAP *image_board;

    // Stores checkers
    std::vector<checker> checkers;

    // Selected coordinates
    int selection_x;
    int selection_y;

    // Stores available moves
    std::vector<position> moves;
};

#endif // BOARD_H
