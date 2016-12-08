#ifndef BOARD_H
#define BOARD_H

// Include allegro for graphics
#include <allegro.h>

// Vector to hold checkers
#include <vector>

// Checkers type
#include "checker.h"

// Holder of board, contains spaces for checkers
class board{
  public:
    board();
    board( int tiles_wide, int tiles_high, int width, int height);
    virtual ~board();

    // Size
    int width;
    int height;
    int tiles_wide;
    int tiles_high;

    // Draw
    void draw( BITMAP *tempBuffer);

    // Add checker to board
    void add_checker( checker newChecker);

    // Check if checker is at position
    bool checker_at( int x, int y);
  protected:

  private:
    // Board generator
    void generate_board();

    // Image of board
    BITMAP *image_board;

    // Stores checkers
    std::vector<checker> checkers;
};

#endif // BOARD_H
