#include "StateColor.h"

StateColor::StateColor() {
  rgb[0] = 0;
  rgb[1] = 0;
  rgb[2] = 0;
}

StateColor::StateColor(char r, char g, char b) {
  rgb[0] = r;
  rgb[1] = g;
  rgb[2] = b;
}

void StateColor::setRgb(char r, char g, char b) {
  rgb[0] = r;
  rgb[1] = g;
  rgb[2] = b;
}
