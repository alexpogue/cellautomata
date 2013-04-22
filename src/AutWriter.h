#ifndef AUT_WRITER_H
#define AUT_WRITER_H

#include "GameGrid.h"
#include <ostream>

class AutWriter {
  public:
    static void write(const GameGrid& gg, std::ostream& file);
};

#endif
