#include "WwSimulator.h"

GameGrid simulateWwGeneration(const GameGrid& gg);
char getNumElecHeadNeighbors(const GameGrid& gg, Point loc);

GameGrid WwSimulator::simulate(const GameGrid& gg, const unsigned int& numGenerations) {
  GameGrid ret(gg);
  for(unsigned int i = 0; i < numGenerations; i++) {
    ret = simulateWwGeneration(ret);
  }
  return ret;
}

GameGrid simulateWwGeneration(const GameGrid& gg) {
  GameGrid dest(gg);
  for(int i = dest.getTerrainBounds().getBottomLeft().getY(); i <= dest.getTerrainBounds().getTopRight().getY(); i++) {
    for(int j = dest.getTerrainBounds().getBottomLeft().getX(); j <= dest.getTerrainBounds().getTopRight().getX(); j++) {
      Point p(j, i);
      char numElecHeads = getNumElecHeadNeighbors(gg, p);
      if(gg.getSquareState(p).getNum() == 0) {
        dest.setSquare(p, 0);
      }
      else if(gg.getSquareState(p).getNum() == 1) {
        dest.setSquare(p, 2);
      }
      else if(gg.getSquareState(p).getNum() == 2) {
        dest.setSquare(p, 3);
      }
      else if(gg.getSquareState(p).getNum() == 3) {
        if(numElecHeads == 1 || numElecHeads == 2) {
          dest.setSquare(p, 1);
        }
        else {
          dest.setSquare(p, 3);
        }
      }
    }
  }
  return dest;
}

char getNumElecHeadNeighbors(const GameGrid& grid, Point loc) {
  char num = 0;
  for(int i = loc.getY() - 1; i <= loc.getY() + 1; i++) {
    for(int j = loc.getX() - 1; j <= loc.getX() + 1; j++) {
      Point check(j, i);
      if(grid.isInBounds(check) && grid.getSquareState(check).getNum() == 1) {
        ++num;
      }
    }
  }
  if(grid.getSquareState(loc).getNum() == 1) {
    --num;
  }
  return num;
}
