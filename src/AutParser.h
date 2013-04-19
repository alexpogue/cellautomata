#ifndef AUT_PARSER_H
#define AUT_PARSER_H

#include "GameGrid.h"

class AutParser {
  public: 
    static void parse(const std::string& rawAutText, GameGrid& gg);
};

#endif
