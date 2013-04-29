#include "GridView.h"
#include <QPen>
#include <QCloseEvent>
#include <QResizeEvent>
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
  float widthRatio = grid.getTerrainBounds().getWidth() / size().width();
  float heightRatio = grid.getTerrainBounds().getHeight() / size().height();
  std::cout << "size().width() = " << size().width() << "\n";
  std::cout << "size().height() = " << size().height() << "\n";
  if(widthRatio > heightRatio) {
    cellSize = float(size().height()) / grid.getTerrainBounds().getHeight();
    std::cout << "using width\n";
  }
  else {
    cellSize = float(size().width()) / grid.getTerrainBounds().getWidth(); 
    std::cout << "using height\n";
  }
  for(unsigned int x = 0; x < grid.getTerrainBounds().getWidth(); x++) {
    scene->addLine(x * cellSize, 0, x * cellSize, cellSize * grid.getTerrainBounds().getHeight(), QPen(Qt::black));
  }
  for(unsigned int y = 0; y < grid.getTerrainBounds().getHeight(); y++) {
    scene->addLine(0, y * cellSize, cellSize * grid.getTerrainBounds().getWidth(), y * cellSize, QPen(Qt::black));
  }
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
