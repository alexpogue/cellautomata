#ifndef GRID_SQUARE_H
#define GRID_SQUARE_H

#include "Point.h"
#include "CellState.h"

class GridSquare {
  private:
    CellState state;
  public:
    GridSquare();
    void setState(const CellState& state);
    CellState getState() const;
};

#endif
