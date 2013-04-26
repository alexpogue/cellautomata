#ifndef TUI_DISPLAY
#define TUI_DISPLAY

#include "GridDisplay.h"
#include "GameGrid.h"

class TuiDisplay : public GridDisplay {
    Rect displayBounds;
    int generation;
    int delay;
    void drawTopBorder(int yPos, int width);
    void drawLeftBorder(int xPos, int yStart, int height);
    void drawBottomScroll(int yPos, int width);
    void drawRightScroll(int xPos, int yStart, int height);
    void printGrid();
    void printInstructions();
    void printInfoBar();
    void runSimulation();
  public:
    TuiDisplay(GameGrid& g);
    void setDelay(int d);
    virtual void update();
    virtual void open();
    virtual void close();
};

#endif
