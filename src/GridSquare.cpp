#include "GridSquare.h"
#include <iostream>
#include "CellState.h"

GridSquare::GridSquare() : state(0, '~', StateColor()) {
}

void GridSquare::setState(const CellState& newState) {
  state = newState;
}

CellState GridSquare::getState() const {
  return state;
}
