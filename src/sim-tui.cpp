#include <ncurses.h>
#include <stdexcept>
#include <iostream>
#include <limits.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "GameGrid.h"
#include "AutParser.h"
#include <fstream>

void readCommandLineArgs(int, char**, int&, int&, int&, int&, std::string&, bool&);
void stringToIntPair(char*, int&, int&);
void strToInt(int& intConv, const char* string);
void displayHelp(char* cmd);

int main(int argc, char** argv) {
  int txLow, txHigh, tyLow, tyHigh;
  bool dispHelp;
  std::string autFilename;
  txLow = txHigh = tyLow = tyHigh = 0;
  dispHelp = false;
  try {
    readCommandLineArgs(argc, argv, txLow, txHigh, tyLow, tyHigh, autFilename, dispHelp);
  }
  catch(std::runtime_error e) {
    std::cout << e.what() << "\n";
    exit(1);
  }
  if(dispHelp) {
    displayHelp(argv[0]);
  }
  Rect terrainBounds(Point(txLow, tyLow), Point(txHigh, tyHigh));
  std::vector<CellState> gameStates;
  gameStates.push_back(CellState(0, '~', StateColor()));
  gameStates.push_back(CellState(1, '1', StateColor()));
  GameGrid gg;
  gg.setGameStates(gameStates);
  try {
    AutParser::parse(autFilename, gg);
  }
  catch(std::ifstream::failure e) {
    std::cout << "Could not open file: " << autFilename << "\n";
    exit(1);
  }
  /* TODO: replace hardcoded values with variables according to actual screen size */
  gg.setWindowBounds(Rect(Point(-19,-7),Point(19,8)));
  gg.printToFile(std::cout, false);
  /*
  initscr();
  printw("hello, world!");
  refresh();
  getch();
  endwin();
  */
  return 0;
}

void readCommandLineArgs(int argc, char** argv, int& txLow, int& txHigh, int& tyLow, int& tyHigh, std::string& fileIn, bool& dispHelp) {
  if(argc <= 1) {
    throw std::runtime_error("error: not enough command line arguments (-h for usage)");
  }
  for(int i = 1; i < argc; i++) {
    if(strncmp(argv[i], "-h", 3) == 0) {
      dispHelp = true;
      return;
    }
    if(strncmp(argv[i], "-tx", 4) == 0 || strncmp(argv[i], "-ty", 4) == 0) {
      if(argc <= i + 1) {
        throw std::runtime_error(std::string("error: no values given for flag: ") + argv[i]);
      }
      if(argv[i][2] == 'x') {
        stringToIntPair(argv[i+1], txLow, txHigh);
      }
      else if(argv[i][2] == 'y') {
        stringToIntPair(argv[i+1], tyLow, tyHigh);
      }
      /* skip int pair */
      ++i;
    }
    else {
      fileIn = argv[i];
    }
  }
}

void stringToIntPair(char* str, int& first, int& second) {
  char* secondStr = strchr(str, ',');
  if(secondStr == NULL || *(secondStr+1) == 0) {
    throw std::runtime_error(std::string("error: not enough values for terrain: expected two-integer range int,int (given: ") + str + ")");
  }
  *secondStr = 0;
  ++secondStr; // increment to after the comma
  strToInt(first, str);
  strToInt(second, secondStr);
}

void strToInt(int& intConv, const char* string) {
  char* afterNum;
  long longConv;
  errno = 0;
  longConv = strtol(string, &afterNum, 10);
  if( (errno == ERANGE && longConv == LONG_MAX) || longConv > INT_MAX) {
    throw std::runtime_error(std::string("error: overflow in integer conversion attempt: '") + string + "'");
  }
  else if( (errno == ERANGE && longConv == LONG_MIN) || longConv < INT_MIN) {
    throw std::runtime_error(std::string("error: underflow in integer conversion attempt: '") + string + "'");
  }
  else if(string[0] == '\0' || afterNum[0] != '\0') {
    throw std::runtime_error(std::string("error: invalid integer: '") + string + "'");
  }
  intConv = longConv;
}

void displayHelp(char* cmd) {
  std::cout << "sim-tui Help:\n";
  std::cout << "Description:\n";
  std::cout << "sim-tui provides a text-based visualization of various cellular automata simulations\n\n";
  std::cout << "Usage: " << cmd << " [options] inputFile.aut\n\n";
  std::cout << "Options:\n";
  std::cout << "-h \t\tDisplay this help screen\n";
  std::cout << "-tx l,h\t\tSet the x-axis terrain range to [l, h] (inclusive)\n";
  std::cout << "-ty l,h\t\tSet the y-axis terrain range to [l, h] (inclusive)\n";
}
