#ifndef BB_SIMULATOR_H
#define BB_SIMULATOR_H

#include "GameGrid.h"

class BbSimulator {
  public:
    static GameGrid simulate(const GameGrid& gg, const unsigned int& numGenerations);
};

#endif
