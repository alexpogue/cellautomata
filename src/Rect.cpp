#include "Rect.h"
#include <iostream>

Rect::Rect(const Point& bottomLeft, const Point& topRight) {
  bLeft = bottomLeft;
  tRight = topRight;
}

Rect::Rect() {
  bLeft = Point(0, 0);
  tRight = Point(0, 0);
}

void Rect::setBottomLeft(Point p) {
  bLeft = p;
}

void Rect::setTopRight(Point p) {
  tRight = p;
}

Point Rect::getBottomLeft() const {
  return bLeft;
}

Point Rect::getTopRight() const {
  return tRight;
}

unsigned int Rect::getWidth() const {
  return tRight.getX() - bLeft.getX();
}

unsigned int Rect::getHeight() const {
  return tRight.getY() - bLeft.getY();
}
