#ifndef RECT_H
#define RECT_H

#include "Point.h"

class Rect {
  private:
    Point bLeft;
    Point tRight;
  public:
    Rect();
    Rect(const Point& bottomLeft, const Point& topRight);
    void setBottomLeft(Point p);
    void setTopRight(Point p);
    Point getBottomLeft() const;
    Point getTopRight() const; 
    unsigned int getWidth() const;
    unsigned int getHeight() const;
};

#endif
