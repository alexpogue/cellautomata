#ifndef GRID_SQUARE_H
#define GRID_SQUARE_H

#include "Point.h"

class GridSquare {
  private:
    Point location;
    bool alive;
    void initialize(const Point& p, const bool& isAlive);
  public:
    GridSquare(const Point& p, const bool& isAlive);
    GridSquare(const Point& p);
    void setAlive(const bool& isAlive);
    bool isAlive() const;
    Point getLocation() const;
};

#endif
