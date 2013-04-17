#ifndef AUT_PARSER_H
#define AUT_PARSER_H

#include "GameGrid.h"

struct ParseData {
  std::vector<Point> aliveCells;
  Rect terrain;
};

class AutParser {
  public: 
    static void parse(const std::string& rawAutText, ParseData& pd);    
};

#endif
