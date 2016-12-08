#ifndef CHECKER_H
#define CHECKER_H

// Allegro included for drawing
#include <allegro.h>

// Checker class
class checker{
  public:
    // Constructor
    checker();

    // Deconstructor
    virtual ~checker();

    // Draw
    void draw( BITMAP *tempBuffer);

  protected:

  private:
};

#endif // CHECKER_H
