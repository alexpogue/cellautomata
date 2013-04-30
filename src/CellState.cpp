#include "CellState.h"
#include "StateColor.h"

CellState::CellState(unsigned char num) {
  stateNum = num;
}

CellState::CellState(unsigned char num, char charRep, const StateColor& color) {
  stateNum = num;
  charRepresentation = charRep;
  colorRepresentation = color;
}

unsigned char CellState::getNum() const {
  return stateNum;
}

char CellState::getChar() const {
  return charRepresentation;
}

void CellState::setChar(char charRep) {
  charRepresentation = charRep;
}

void CellState::setColor(const StateColor& color) {
  colorRepresentation = color;
}

StateColor CellState::getColor() const {
  return colorRepresentation;
}
