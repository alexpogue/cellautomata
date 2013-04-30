#include "GridView.h"
#include <QPen>
#include <QCloseEvent>
#include <QResizeEvent>
#include <QGraphicsRectItem>
#include "GameGrid.h"
#include "GolSimulator.h"
#include "BbSimulator.h"
#include "WwSimulator.h"
#include "LaSimulator.h"
#include "GameRules.h"

GridView::GridView(GameGrid& g, QWidget* parent) 
  : QGraphicsView(parent),
    grid(g) {
  setWindowTitle(grid.getName().c_str());
  currentScaleFactor = 1;
  scene = new QGraphicsScene;
  setScene(scene);
  drawGrid();
}

void GridView::displayNextGen() {
  if(grid.getRules() == RULES_CONWAYS_LIFE) {
    grid = GolSimulator::simulate(grid, 1); 
  }
  else if(grid.getRules() == RULES_BRIANS_BRAIN) {
    grid = BbSimulator::simulate(grid, 1);
  }
  else if(grid.getRules() == RULES_WIRE_WORLD) {
    grid = WwSimulator::simulate(grid, 1);
  }
  else if(grid.getRules() == RULES_LANGTONS_ANT) {
    grid = LaSimulator::simulate(grid, 1);
  }
  scene->clear();
  drawGrid();
}

void GridView::drawGrid() {
  float cellSize;
  cellSize = float(size().width()) / grid.getTerrainBounds().getWidth(); 
  for(unsigned int x = 0; x < grid.getTerrainBounds().getWidth(); x++) {
    for(unsigned int y = 0; y < grid.getTerrainBounds().getHeight(); y++) {
      Point normalized(x + grid.getTerrainBounds().getBottomLeft().getX(), y + grid.getTerrainBounds().getBottomLeft().getY());
      int cellTop = size().height() - y * cellSize - cellSize;
      int cellLeft = x * cellSize;
      QGraphicsRectItem* rect = new QGraphicsRectItem(cellLeft, cellTop, cellSize, cellSize);
      StateColor color = grid.getSquareState(normalized).getColor();
      if(grid.getSquareState(normalized).getNum() == 0) {
        rect->setBrush(QBrush(QColor(color.red, color.green, color.blue)));
      }
      if(grid.getSquareState(normalized).getNum() == 1) {
        rect->setBrush(QBrush(QColor(color.red, color.green, color.blue)));
      }
      scene->addItem(rect);
    }
  }
}

void GridView::changeZoom(int factor) {
  setTransform(QTransform::fromScale(factor, factor));
}

void GridView::closeEvent(QCloseEvent* event) {
  emit gridClosed();
  event->accept();
}

void GridView::resizeEvent(QResizeEvent*) {
  scene->clear();
  drawGrid();
}
