#include "Point.h"
#include <iostream>

Point::Point() {
  x = 0;
  y = 0;
}

Point::Point(const Point& other) {
  x = other.x;
  y = other.y;
}

Point::Point(const int& xIn, const int& yIn) {
  x = xIn;
  y = yIn;
}

void Point::setX(const int& xIn) {
  x = xIn;
}

void Point::setY(const int& yIn) {
  y = yIn;
}

int Point::getX() const {
  return x;
}

int Point::getY() const {
  return y;
}
