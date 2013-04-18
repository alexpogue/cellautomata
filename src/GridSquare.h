#ifndef GRID_SQUARE_H
#define GRID_SQUARE_H

#include "Point.h"

class GridSquare {
  private:
    bool alive;
  public:
    GridSquare();
    void setAlive(const bool& isAlive);
    bool isAlive() const;
};

#endif
