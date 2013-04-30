#include "LaSimulator.h"

struct AntToAdd {
  int direction; // 0 = N, 1 = E, 2 = S, 3 = W
  Point loc;
};

GameGrid simulateLaGeneration(const GameGrid& gg);

GameGrid LaSimulator::simulate(const GameGrid& gg, const unsigned int& numGenerations) {
  GameGrid ret(gg);
  for(unsigned int i = 0; i < numGenerations; i++) {
    ret = simulateLaGeneration(ret);
  }
  return ret;
}

GameGrid simulateLaGeneration(const GameGrid& gg) {
  GameGrid dest(gg);
  std::vector<AntToAdd> nextGenAnts;
  for(int i = dest.getTerrainBounds().getBottomLeft().getY(); i <= dest.getTerrainBounds().getTopRight().getY(); i++) {
    for(int j = dest.getTerrainBounds().getBottomLeft().getX(); j <= dest.getTerrainBounds().getTopRight().getX(); j++) {
      Point p(j, i);
      if(gg.getSquareState(p).getNum() == 0) {
        dest.setSquare(p, 0);
      }
      else if(gg.getSquareState(p).getNum() == 1) {
        dest.setSquare(p, 1);
      }
      else if(gg.getSquareState(p).getNum() == 2) {
        dest.setSquare(p, 1);
        AntToAdd nextAnt;
        nextAnt.direction = 1;
        nextAnt.loc = Point(p.getX() + 1, p.getY());
        nextGenAnts.push_back(nextAnt);
      }
      else if(gg.getSquareState(p).getNum() == 3) {
        dest.setSquare(p, 1);
        AntToAdd nextAnt;
        nextAnt.direction = 2;
        nextAnt.loc = Point(p.getX(), p.getY() - 1);
        nextGenAnts.push_back(nextAnt);
      }
      else if(gg.getSquareState(p).getNum() == 4) {
        dest.setSquare(p, 1);
        AntToAdd nextAnt;
        nextAnt.direction = 3;
        nextAnt.loc = Point(p.getX() - 1, p.getY());
        nextGenAnts.push_back(nextAnt);
      }
      else if(gg.getSquareState(p).getNum() == 5) {
        dest.setSquare(p, 1);
        AntToAdd nextAnt;
        nextAnt.direction = 0;
        nextAnt.loc = Point(p.getX(), p.getY() + 1);
        nextGenAnts.push_back(nextAnt);
      }
      else if(gg.getSquareState(p).getNum() == 6) {
        dest.setSquare(p, 0);
        AntToAdd nextAnt;
        nextAnt.direction = 3;
        nextAnt.loc = Point(p.getX() - 1, p.getY());
        nextGenAnts.push_back(nextAnt);
      }
      else if(gg.getSquareState(p).getNum() == 7) {
        dest.setSquare(p, 0);
        AntToAdd nextAnt;
        nextAnt.direction = 0;
        nextAnt.loc = Point(p.getX(), p.getY() + 1);
        nextGenAnts.push_back(nextAnt);
      }
      else if(gg.getSquareState(p).getNum() == 8) {
        dest.setSquare(p, 0);
        AntToAdd nextAnt;
        nextAnt.direction = 1;
        nextAnt.loc = Point(p.getX() + 1, p.getY());
        nextGenAnts.push_back(nextAnt);
      }
      else if(gg.getSquareState(p).getNum() == 9) {
        dest.setSquare(p, 0);
        AntToAdd nextAnt;
        nextAnt.direction = 2;
        nextAnt.loc = Point(p.getX(), p.getY() - 1);
        nextGenAnts.push_back(nextAnt);
      }
    }
  }
  for(unsigned int i = 0; i < nextGenAnts.size(); i++) {
    int currState = dest.getSquareState(nextGenAnts[i].loc).getNum();
    int nextState;
    if(currState == 0 || currState == 1) {
      /* set direction of ant */
      nextState = currState * 4 + 2 + nextGenAnts[i].direction;
    }
    else {
      /* ant already at the square, annihilate them! */
      if(currState >= 2 && currState <= 5) {
        nextState = 1;
      }
      else if(currState >= 6 && currState <= 9) {
        nextState = 0;
      }
    }
    dest.setSquare(nextGenAnts[i].loc, nextState);
  }
  return dest;
}
