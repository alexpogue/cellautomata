#include "GridView.h"
#include <QPen>
#include <QCloseEvent>
#include <QResizeEvent>
#include <QGraphicsRectItem>
#include "GameGrid.h"
#include "GolSimulator.h"

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
  grid = GolSimulator::simulate(grid, 1); 
  scene->clear();
  drawGrid();
}

void GridView::drawGrid() {
  float cellSize;
  cellSize = float(size().width()) / grid.getTerrainBounds().getWidth(); 
  for(unsigned int x = 0; x < grid.getTerrainBounds().getWidth(); x++) {
    scene->addLine(x * cellSize, 0, x * cellSize, cellSize * grid.getTerrainBounds().getHeight(), QPen(Qt::black));
  }
  for(unsigned int y = 0; y < grid.getTerrainBounds().getHeight(); y++) {
    scene->addLine(0, y * cellSize, cellSize * grid.getTerrainBounds().getWidth(), y * cellSize, QPen(Qt::black));
  }
  for(unsigned int x = 0; x < grid.getTerrainBounds().getWidth(); x++) {
    for(unsigned int y = 0; y < grid.getTerrainBounds().getHeight(); y++) {
      Point normalized(x + grid.getTerrainBounds().getBottomLeft().getX(), y + grid.getTerrainBounds().getBottomLeft().getY());
      int cellTop = y * cellSize;
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
