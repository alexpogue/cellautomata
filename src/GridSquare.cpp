#include "GridSquare.h"
#include <iostream>
#include "CellState.h"

GridSquare::GridSquare() : state(0, '~', StateColor()) {
}

GridSquare::GridSquare(const CellState& s) : state(s.getNum()) {
  StateColor color;
  state.getColor(color);
  state = CellState(state.getNum(), state.getChar(), color);
}

void GridSquare::setState(const CellState& newState) {
  state = newState;
}

CellState GridSquare::getState() const {
  return state;
}
