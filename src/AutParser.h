#ifndef AUT_PARSER_H
#define AUT_PARSER_H

#include "GameGrid.h"

class AutParser {
  public: 
    static GameGrid parse(std::string rawAutText);    
};

#endif
