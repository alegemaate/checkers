#ifndef CHECKER_H
#define CHECKER_H

// Allegro included for drawing
#include <allegro.h>

// Const colors
const int COLOR_RED = 1;
const int COLOR_BLACK = 0;

// Checker class
class checker{
  public:
    // Constructors
    checker();
    checker( int x, int y, int width, int height, int color);

    // Deconstructor
    virtual ~checker();

    // Draw
    void draw( BITMAP *tempBuffer);

    // At position
    bool is_at( int x, int y);

    // Move
    void jump( bool left, bool super);

    // Type
    int type;
    int color;

    // Get x and y
    int get_x(){ return x; }
    int get_y(){ return y; }
  protected:

  private:
    // Image
    BITMAP *image;

    // Position
    int x;
    int y;

    // Size
    int width;
    int height;

    // Generate image
    static BITMAP *generate_image( int width, int height, int color);
};

#endif // CHECKER_H
