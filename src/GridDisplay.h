#ifndef GRID_DISPLAY_H
#define GRID_DISPLAY_H

#include "GameGrid.h"

class GridDisplay {
  protected:
    GameGrid& grid;
    GridDisplay(GameGrid& g);
  public:
    virtual void update() = 0;
    virtual void open() = 0;
    virtual void close() = 0;
};

#endif
