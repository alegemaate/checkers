#ifndef POSITION_H
#define POSITION_H

// Stores 2 dimensional coordinates
class position{
  public:
    position(int x = -1, int y = -1);
    virtual ~position();

    int x;
    int y;
};

#endif
