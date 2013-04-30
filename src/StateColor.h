#ifndef STATE_COLOR_H
#define STATE_COLOR_H

struct StateColor {
  unsigned char red;
  unsigned char green;
  unsigned char blue;
  StateColor();
  StateColor(unsigned char r, unsigned char g, unsigned char b);
  void setRgb(unsigned char r, unsigned char g, unsigned char b);
  void setRed(unsigned char r);
  void setGreen(unsigned char g);
  void setBlue(unsigned char b);
};

#endif
