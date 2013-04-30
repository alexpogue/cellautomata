#include "BbSimulator.h"

GameGrid simulateBbGeneration(const GameGrid& gg);
char getNumOnNeighbors(const GameGrid& gg, Point loc);

GameGrid BbSimulator::simulate(const GameGrid& gg, const unsigned int& numGenerations) {
  GameGrid ret(gg);
  for(unsigned int i = 0; i < numGenerations; i++) {
    ret = simulateBbGeneration(ret);
  }
  return ret;
}

GameGrid simulateBbGeneration(const GameGrid& gg) {
  GameGrid dest(gg);
  for(int i = dest.getTerrainBounds().getBottomLeft().getY(); i <= dest.getTerrainBounds().getTopRight().getY(); i++) {
    for(int j = dest.getTerrainBounds().getBottomLeft().getX(); j <= dest.getTerrainBounds().getTopRight().getX(); j++) {
      Point p(j, i);
      char numOn = getNumOnNeighbors(gg, p);
      if(gg.getSquareState(p).getNum() == 0) {
        if(numOn == 2) {
          dest.setSquare(p, 2);
        }
        else {
          dest.setSquare(p, 0);
        }
      }
      else if(gg.getSquareState(p).getNum() == 1) {
        dest.setSquare(p, 0);
      }
      else if(gg.getSquareState(p).getNum() == 2) {
        dest.setSquare(p, 1);
      }
    }
  }
  return dest;
}

char getNumOnNeighbors(const GameGrid& grid, Point loc) {
  char num = 0;
  for(int i = loc.getY() - 1; i <= loc.getY() + 1; i++) {
    for(int j = loc.getX() - 1; j <= loc.getX() + 1; j++) {
      Point check(j, i);
      if(grid.isInBounds(check) && grid.getSquareState(check).getNum() == 2) {
        ++num;
      }
    }
  }
  if(grid.getSquareState(loc).getNum() == 2) {
    --num;
  }
  return num;
}
