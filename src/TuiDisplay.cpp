#include "TuiDisplay.h"
#include "GridDisplay.h"
#include <ncurses.h>

TuiDisplay::TuiDisplay(GameGrid& g) : 
  GridDisplay(g) {}

void TuiDisplay::open() {
  int rows, cols;
  std::string gridNameTemp = "Grid Name";
  initscr();
  getmaxyx(stdscr, rows, cols);
  displayBounds = Rect(Point(1, 2), Point(cols-2, rows - 3));
  drawTopBorder(2, cols);
  drawLeftBorder(0, 3, displayBounds.getHeight());
  drawBottomScroll(rows-2, cols);
  mvprintw(0, (cols - gridNameTemp.length())/2, "%s", gridNameTemp.c_str());
  printGrid();
  refresh();
}

void TuiDisplay::drawTopBorder(int yPos, int width) {
  std::string borderStr;
  borderStr = "+";
  for(int i = 1; i < width - 1; i++) {
    borderStr.push_back('-');
  }
  borderStr.push_back('+');
  mvprintw(yPos, 0, "%s", borderStr.c_str());
}

void TuiDisplay::drawLeftBorder(int xPos, int yStart, int height) {
  for(int i = yStart; i < height + yStart - 1; i++) {
    mvprintw(i, xPos, "|");
  }
}

void TuiDisplay::drawBottomScroll(int yPos, int width) {
  int charsInScroller = width - 4;
}

void TuiDisplay::update() {
  printGrid();
  refresh();
}

void TuiDisplay::printGrid() {
  int rows, cols;
  getmaxyx(stdscr, rows, cols);
  Rect t(grid.getTerrainBounds());
  Point tbl(t.getBottomLeft());
  Point ttr(t.getTopRight());
  Point dbl(tbl.getX(), ttr.getY() - displayBounds.getHeight() + 1);
  Point dtr(tbl.getX() + displayBounds.getWidth() - 1, ttr.getY());
  grid.setWindowBounds(Rect(dbl, dtr));
  Point startDrawing(displayBounds.getBottomLeft().getX(), rows - displayBounds.getTopRight().getY());
  for(int i = displayBounds.getTopRight().getY(); i > displayBounds.getBottomLeft().getY(); i--) {
    int currRowDown = displayBounds.getTopRight().getY() - i;
    int currGridRow = grid.getWindowBounds().getTopRight().getY() - currRowDown;
    mvprintw(startDrawing.getY() + (displayBounds.getTopRight().getY() - i), startDrawing.getX(), "%s", grid.getRowAsciiString(currGridRow).c_str());
  }
}

void TuiDisplay::close() {
  endwin();
}
