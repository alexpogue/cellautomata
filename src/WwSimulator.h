#ifndef WW_SIMULATOR_H
#define WW_SIMULATOR_H

#include "GameGrid.h"

class WwSimulator {
  public:
    static GameGrid simulate(const GameGrid& gg, const unsigned int& numGenerations);
};

#endif
