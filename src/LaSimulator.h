#ifndef LA_SIMULATOR_H
#define LA_SIMULATOR_H

#include "GameGrid.h"

class LaSimulator {
  public:
    static GameGrid simulate(const GameGrid& gg, const unsigned int& numGenerations);
};

#endif
