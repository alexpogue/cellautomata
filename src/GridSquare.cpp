#include "GridSquare.h"
#include <iostream>

GridSquare::GridSquare() {
  alive = false;
}

void GridSquare::setAlive(const bool& isAlive) {
  alive = isAlive;
}

bool GridSquare::isAlive() const {
  return alive;
}
