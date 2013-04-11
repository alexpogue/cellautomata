#ifndef GAME_GRID_H
#define GAME_GRID_H

/**
  Grid 2D array representation:

  __|_0_|_1_|_2_|_3_|  
  0 |   |   |   |   |   |
  1 |   |   |   |   |   |  grid
  2 |   |   |   |   |  \|/ values
  3 |___|___|___|___|   v  increase

  ------------------->
  grid values increase

  The grid will look upside down in this portrayal (printed coord system has 0,0
  in bottom left), but this method saves us from inverting the coordinates when 
  accessing elements. When printing, we'll have to print backward (last row to 
  first row)
*/

#include "GridSquare.h"
#include "Point.h"
#include "Rect.h"
#include <ostream>
#include <vector>

class GameGrid {
  private:
    std::vector< std::vector<GridSquare> > grid;
    Rect terrainBounds;
    Rect windowBounds;
    void fillWithZeros();
    void printRow(const int& row, std::ostream& outStream, const bool& autOutput) const;
    void initialize(const Rect& terrainBounds, const Rect& windowBounds);
  public:
    GameGrid();
    GameGrid(const Rect& terrainBounds);
    GameGrid(const Rect& terrainBounds, const Rect& windowBounds);
    unsigned int getTerrainHeight() const;
    unsigned int getTerrainWidth() const;
    unsigned int getWindowHeight() const;
    unsigned int getWindowWidth() const;
    void setTerrainBounds(const Rect& bounds);
    void setWindowBounds(const Rect& bounds);
    void printToFile(std::ostream& out, const bool& autOutput) const;
    void setSquare(const Point& p, const bool& alive);
    bool isSquareAlive(const Point& p) const;
};

#endif
