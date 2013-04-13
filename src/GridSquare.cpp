#include "GridSquare.h"
#include <iostream>

GridSquare::GridSquare(const Point& p, const bool& isAlive) {
  initialize(p, isAlive);
}

GridSquare::GridSquare(const Point& p) {
  initialize(p, false);
}

GridSquare::GridSquare() {
  initialize(Point(0, 0), false);
}

void GridSquare::initialize(const Point& p, const bool& isAlive) {
  location = p;
  alive = isAlive;
}

void GridSquare::setAlive(const bool& isAlive) {
  alive = isAlive;
}

bool GridSquare::isAlive() const {
  return alive;
}

Point GridSquare::getLocation() const {
  return location;
}
