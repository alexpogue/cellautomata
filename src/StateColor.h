#ifndef STATE_COLOR_H
#define STATE_COLOR_H

struct StateColor {
  unsigned char rgb[3];
  StateColor();
  StateColor(char r, char g, char b);
  void setRgb(char r, char g, char b);
};

#endif
