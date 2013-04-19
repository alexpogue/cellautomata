#ifndef GOL_SIMULATOR_H
#define GOL_SIMULATOR_H

#include "GameGrid.h"

class GolSimulator {
  public:
    static GameGrid simulate(const GameGrid& gg, const unsigned int& numGenerations);
};

#endif
