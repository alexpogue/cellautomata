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
    void printGrid();
    void printInstructions();
    void printInfoBar(int,int);
  public:
    TuiDisplay(GameGrid& g);
    virtual void update();
    virtual void open();
    virtual void close();
};

#endif
