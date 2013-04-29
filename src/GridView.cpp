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
/*  for(int x = 0; x < size().width(); x += 10) {
    scene->addLine(x, 0, x, size().height(), QPen(Qt::black));
  }
  for(int y = 0; y < size().height(); y += 10) {
    scene->addLine(0, y, size().width(), y, QPen(Qt::black));
  }
*/
}

void GridView::displayNextGen() {
  grid = GolSimulator::simulate(grid, 1); 
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
  /* EXAMPLE DRAWING RECT
  QGraphicsRectItem* rect = new QGraphicsRectItem(0,0,cellSize, cellSize);
  rect->setBrush(QBrush(Qt::black));
  scene->addItem(rect);
  /* BAD CODE, FIX
  for(unsigned int x = 0; x < grid.getTerrainBounds().getWidth(); x++) {
    for(unsigned int y = 0; y < grid.getTerrainBounds().getHeight(); y++) {
      if(grid.getSquareState(Point(x, y)).getNum() == 1) {
        int yHigh = size().height() - grid.getTerrainBounds().getTopRight().getY() * cellSize;
        int yLow = size().height() - grid.getTerrainBounds().getBottomLeft().getY() * cellSize;
        std::cout << "Here!\n";
        std::cout << "yHigh = " << yHigh << ", xHigh = " << yLow << "\n";
        QGraphicsRectItem* rect = new QGraphicsRectItem(x * cellSize, yLow, x * cellSize + cellSize, yHigh);
        rect->setBrush(QBrush(Qt::black));
        scene->addItem(rect);
      }
    }
  }
  */
}

void GridView::changeZoom(int factor) {
  setTransform(QTransform::fromScale(factor, factor));
}

void GridView::closeEvent(QCloseEvent* event) {
  emit gridClosed();
  event->accept();
}

void GridView::resizeEvent(QResizeEvent* event) {
  scene->clear();
  drawGrid();
}
