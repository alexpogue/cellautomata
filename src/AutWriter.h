#ifndef AUT_WRITER_H
#define AUT_WRITER_H

#include "GameGrid.h"

class AutWriter {
  public:
    static void write(const GameGrid& gg, const std::ostream& file);
};

#endif
