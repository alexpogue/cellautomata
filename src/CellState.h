#ifndef CELL_STATE_H
#define CELL_STATE_H

#include "StateColor.h"

class CellState {
    unsigned char stateNum;
    char charRepresentation;
    StateColor colorRepresentation; 
  public:
    CellState(unsigned char num);
    CellState(unsigned char num, char charRep, const StateColor& color);
    unsigned char getNum() const;
    void setChar(char charRep);
    char getChar() const;
    void setColor(const StateColor& color);
    void getColor(StateColor& color) const;
};

#endif
