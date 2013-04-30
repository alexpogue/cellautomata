#ifndef TUI_DISPLAY
#define TUI_DISPLAY

#include "GameGrid.h"

class TuiDisplay {
    GameGrid& grid;
    Rect displayBounds;
    int generation;
    int delay;
    bool paused;
    void drawTopBorder(int yPos, int width);
    void drawLeftBorder(int xPos, int yStart, int height);
    void drawBottomScroll(int yPos, int width);
    void drawRightScroll(int xPos, int yStart, int height);
    void printGrid();
    void printInstructions();
    void printInfoBar();
    void runSimulation();
    void scrollRight();
    void scrollLeft();
    void scrollUp();
    void scrollDown();
    void handleInput(int c);
    void updateDelay(int delta);
  public:
    TuiDisplay(GameGrid& g);
    void setDelay(int d);
    virtual void update();
    virtual void open();
    virtual void close();
};

#endif
