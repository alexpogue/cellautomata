#ifndef AUT_PARSER_H
#define AUT_PARSER_H

#include "GameGrid.h"
#include <istream>
#include <string>

class AutParser {
  public: 
    static void parse(std::istream& file, GameGrid& gg);
};

#endif
