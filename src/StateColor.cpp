#include "StateColor.h"

StateColor::StateColor() {
  red = 0;
  green = 0;
  blue = 0;
}

StateColor::StateColor(unsigned char r, unsigned char g, unsigned char b) {
  red = r;
  green = g;
  blue = b;
}

void StateColor::setRgb(unsigned char r, unsigned char g, unsigned char b) {
  red = r;
  green = g;
  blue = b;
}
