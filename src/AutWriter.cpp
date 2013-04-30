#include "AutWriter.h"

void AutWriter::write(const GameGrid& gg, std::ostream& out) {
  out << "Name \"" << gg.getName() << "\";\n";
  if(gg.getNumCharsFromAut() > 0) {
    out << "Chars ";
    for(int i = 0; i < gg.getNumCharsFromAut(); i++) {
      out << (int)gg.getGameStates()[i].getChar();
      if(i != gg.getNumCharsFromAut() - 1) out << ", ";
    }
    out << ";\n";
  }
  if(gg.getNumColorsFromAut() > 0) {
    out << "Colors ";
    for(int i = 0; i < gg.getNumColorsFromAut(); i++) {
      out << "(";
      for(int j = 0; j < 3; j++) {
        if(j == 0) out << (int)gg.getGameStates()[i].getColor().red;
        else if(j == 1) out << (int)gg.getGameStates()[i].getColor().green;
        else if(j == 2) out << (int)gg.getGameStates()[i].getColor().blue;
        if(j != 2) out << ", ";
      }
      out << ")";
      if(i != gg.getNumColorsFromAut() - 1) out << ", ";
    }
    out << ";\n";
  }
  out << "Xrange " << gg.getTerrainBounds().getBottomLeft().getX() << " " << gg.getTerrainBounds().getTopRight().getX() << ";\n";
  out << "Yrange " << gg.getTerrainBounds().getBottomLeft().getY() << " " << gg.getTerrainBounds().getTopRight().getY() << ";\n";
  out << "Initial {\n";
  for(int i = gg.getWindowBounds().getBottomLeft().getY(); i <= gg.getWindowBounds().getTopRight().getY(); i++) {
    bool hasMarkedCell = false;
    for(int j = gg.getWindowBounds().getBottomLeft().getX(); j <= gg.getWindowBounds().getTopRight().getX(); j++) {
      if(gg.getSquareState(Point(j, i)).getNum() == 1) {
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
