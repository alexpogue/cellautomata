#include "TuiDisplay.h"
#include "GridDisplay.h"
#include <stdexcept>
#include <vector>
#include <ncurses.h>

TuiDisplay::TuiDisplay(GameGrid& g) : 
  GridDisplay(g) {}

void TuiDisplay::open() {
  int rows, cols;
  std::string gridNameTemp = "Grid Name";
  initscr();
  getmaxyx(stdscr, rows, cols);
  if(rows < 20 || cols < 40) {
    throw std::runtime_error("Not enough space to draw, increase terminal window size");
  }
  displayBounds = Rect(Point(1, 2), Point(cols-2, rows - 3));
  Point windowBottomLeft(grid.getTerrainBounds().getBottomLeft().getX(), grid.getTerrainBounds().getTopRight().getY() - displayBounds.getHeight() + 1);
  Point windowTopRight(grid.getTerrainBounds().getBottomLeft().getX() + displayBounds.getWidth() - 1, grid.getTerrainBounds().getTopRight().getY());
  grid.setWindowBounds(Rect(windowBottomLeft, windowTopRight));
  drawTopBorder(2, cols);
  drawLeftBorder(0, 3, displayBounds.getHeight());
  drawLeftBorder(cols-1, 3, displayBounds.getHeight());
  mvprintw(0, (cols - gridNameTemp.length())/2, "%s", gridNameTemp.c_str());
  drawBottomScroll(rows-2, cols);
  printGrid();
  printInstructions();
  printInfoBar(100, 0);
  refresh();
  getch();
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
  int numCharsInScrollTrack = width - 4;
  mvprintw(yPos, 0, "+<");
  int charsUsedSoFar = 2;
  mvprintw(yPos, width-2, ">+");
  if(grid.getTerrainBounds().getBottomLeft().getX() < grid.getWindowBounds().getBottomLeft().getX()) {
    int colsLeftOfWindow = grid.getWindowBounds().getBottomLeft().getX() - grid.getTerrainBounds().getBottomLeft().getX();
    float ratio = float(colsLeftOfWindow) / grid.getTerrainBounds().getWidth();
    int numCharsLeftScrollTrack = ratio * numCharsInScrollTrack;
    mvprintw(yPos, charsUsedSoFar, std::string(numCharsLeftScrollTrack, '-').c_str());
    charsUsedSoFar += numCharsLeftScrollTrack;
  }
  if(grid.getTerrainBounds().getWidth() > grid.getWindowBounds().getWidth()) {
    float ratio = float(grid.getWindowBounds().getWidth()) / grid.getTerrainBounds().getWidth();
    mvprintw(1,0,"%f",ratio);
    int numCharsInScrollBar = ratio * numCharsInScrollTrack; 
    mvprintw(yPos, charsUsedSoFar, std::string(numCharsInScrollBar, '#').c_str());
    charsUsedSoFar += numCharsInScrollBar;
  }
  if(grid.getTerrainBounds().getTopRight().getX() > grid.getWindowBounds().getTopRight().getX()) {
    int colsRightOfWindow = grid.getTerrainBounds().getTopRight().getX() - grid.getWindowBounds().getTopRight().getX();
    float ratio = float(colsRightOfWindow) / grid.getTerrainBounds().getWidth();
    mvprintw(0,0,"%f",ratio);
    int numCharsRightScrollTrack = ratio * numCharsInScrollTrack;
    mvprintw(yPos, charsUsedSoFar, std::string(numCharsRightScrollTrack, '-').c_str());
    charsUsedSoFar += numCharsRightScrollTrack;
  }
}

void TuiDisplay::update() {
  printGrid();
  refresh();
}

void TuiDisplay::printGrid() {
  int rows, cols;
  getmaxyx(stdscr, rows, cols);
  for(int i = displayBounds.getTopRight().getY(); i > displayBounds.getBottomLeft().getY(); i--) {
    int currRowDown = displayBounds.getTopRight().getY() - i;
    int currGridRow = grid.getWindowBounds().getTopRight().getY() - currRowDown;
    mvprintw(rows - i, displayBounds.getBottomLeft().getX(), "%s", grid.getRowAsciiString(currGridRow).c_str());
  }
}

void TuiDisplay::printInstructions() {
  static const std::string arr[] = {"(Q)uit", "(P)lay", "(S)tep", "Arrows:scroll"};
  std::vector<std::string> instructions(arr, arr + sizeof(arr) / sizeof(arr[0]));
  int rows, cols, numInstructChars;
  numInstructChars = 0;
  for(unsigned int i = 0; i < instructions.size(); i++) {
    numInstructChars += instructions[i].size();
  }
  getmaxyx(stdscr, rows, cols);
  int numSpaces = cols - numInstructChars;
  int numPartitions = instructions.size() - 1;
  int spacesPerPartition = numSpaces / numPartitions;
  int currPos = 0;
  mvprintw(rows-1, currPos, "%s", instructions[0].c_str());
  currPos += instructions[0].size() + spacesPerPartition;
  for(unsigned int i = 1; i < instructions.size(); i++) {
    mvprintw(rows-1, currPos, "%s", instructions[i].c_str());
    currPos += instructions[i].size() + spacesPerPartition;
  }
}

void TuiDisplay::printInfoBar(int delay, int generation) {
  int rows, cols;
  getmaxyx(stdscr, rows, cols);
  mvprintw(1, 0, "Delay: %d (+/-)", delay);
  mvprintw(1, cols - 18, "Generation: %d", generation);
}

void TuiDisplay::close() {
  endwin();
}
