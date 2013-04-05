#ifndef POINT_H
#define POINT_H

class Point {
  private:
    int x;
    int y;
  public:
    Point();
    Point(const Point& other);
    Point(const int& x, const int& y);
    void setX(const int& xIn);
    void setY(const int& yIn);
    int getX() const;
    int getY() const;
};

#endif
