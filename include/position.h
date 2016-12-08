#ifndef POSITION_H
#define POSITION_H

// Stores 2 dimensional coordinates
class position{
  public:
    position(int x, int y);
    virtual ~position();

    int x;
    int y;
};

#endif
