#ifndef TUI_DISPLAY
#define TUI_DISPLAY

#include "GridDisplay.h"
#include "GameGrid.h"

class TuiDisplay : public GridDisplay {
    Rect displayBounds;
    void drawTopBorder(int yPos, int width);
    void drawLeftBorder(int xPos, int yStart, int height);
    void printGrid();
  public:
    TuiDisplay(GameGrid& g);
    virtual void update();
    virtual void open();
    virtual void close();
};

#endif
