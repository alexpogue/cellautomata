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
    std::vector<CellState> gameStates;
    std::string name;
    Rect terrainBounds;
    Rect windowBounds;
    void printRow(const int& row, std::ostream& outStream) const;
    void initialize(const Rect& terrainBounds, const Rect& windowBounds);
    void serializePoint(unsigned int& serialX, unsigned int& serialY, const Point& p) const;
    void normalizePoint(Point& pSerial, const unsigned int& normX, const unsigned int& normY) const;
    void changeRowsTop(const int& delta);
    void changeRowsBottom(const int& delta);
    void changeColsLeft(const int& delta);
    void changeColsRight(const int& delta);
    void addRowsTop(const unsigned int& numRows);
    void addRowsBottom(const unsigned int& numRows);
    void addColsRight(const unsigned int& numCols);
    void addColsLeft(const unsigned int& numCols);
    void removeRowsTop(const unsigned int& numRows);
    void removeRowsBottom(const unsigned int& numRows);
    void removeColsRight(const unsigned int& numCols);
    void removeColsLeft(const unsigned int& numCols);

  public:
    GameGrid();
    GameGrid(const Rect& terrainBounds);
    GameGrid(const Rect& terrainBounds, const Rect& windowBounds);
    GameGrid(const Rect& terrain, const std::vector<CellState>& states);
    GameGrid(const Rect& terrain, const Rect& window, const std::vector<CellState>& states);
    void initialize(const Rect& terrain, const Rect& window, const std::vector<CellState>& states);
    void resetGrid();
    std::string getName() const;
    void setName(const std::string& n);
    unsigned int getTerrainHeight() const;
    unsigned int getTerrainWidth() const;
    unsigned int getWindowHeight() const;
    unsigned int getWindowWidth() const;
    Rect getTerrainBounds() const;
    Rect getWindowBounds() const;
    void setTerrainBounds(const Rect& bounds);
    void setWindowBounds(const Rect& bounds);
    void printToFile(std::ostream& out, const bool& autOutput) const;
    std::string getAsciiString();
    std::string getRowAsciiString(int row);
    void setSquare(const Point& p, int alive);
    CellState getSquareState(const Point& p) const;
    bool isInBounds(const Point& p) const;
    void setGameStates(const std::vector<CellState>& states);
    void setGameStates(const std::string& charStates);
    void refillStates();
};

#endif
