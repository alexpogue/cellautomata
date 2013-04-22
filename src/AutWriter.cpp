#include "AutWriter.h"

void AutWriter::write(const GameGrid& gg, std::ostream& out) {
  out << "Xrange " << gg.getTerrainBounds().getBottomLeft().getX() << " " << gg.getTerrainBounds().getTopRight().getX() << ";\n";
  out << "Yrange " << gg.getTerrainBounds().getBottomLeft().getY() << " " << gg.getTerrainBounds().getTopRight().getY() << ";\n";
  out << "Initial {\n";
  for(int i = gg.getWindowBounds().getBottomLeft().getY(); i <= gg.getWindowBounds().getTopRight().getY(); i++) {
    bool hasMarkedCell = false;
    for(int j = gg.getWindowBounds().getBottomLeft().getX(); j <= gg.getWindowBounds().getTopRight().getX(); j++) {
      if(gg.isSquareAlive(Point(j, i))) {
        if(!hasMarkedCell) {
          out << "  Y = " << i << " : " << j;
          hasMarkedCell = true;
        }
        else {
          out << ", " << j;
        }
      }
    }
    if(hasMarkedCell) {
      out << ";\n";
    }
  }
  out << "};\n";
}
