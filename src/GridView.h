#ifndef GRID_VIEW
#define GRID_VIEW

#include <QGraphicsView>

class QGraphicsScene;
class QCloseEvent;
class QResizeEvent;
class GameGrid;

class GridView : public QGraphicsView {
    Q_OBJECT
  public:
    GridView(GameGrid& g, QWidget* parent = 0);
  signals:
    void gridClosed();
  public slots:
    void displayNextGen();
    void changeZoom(int factor);
  private:
    void closeEvent(QCloseEvent* event);
    void resizeEvent(QResizeEvent* event);
    QGraphicsScene* scene;
    void drawGrid();
    int currentScaleFactor;
    GameGrid& grid;
};
    
#endif
