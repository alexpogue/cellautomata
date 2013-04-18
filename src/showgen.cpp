#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <climits>
#include <cerrno>
#include "GameGrid.h"
#include "Rect.h"
#include "Point.h"

typedef enum {
  CONVERSION_SUCCESS,
  CONVERSION_MEMORY,
  CONVERSION_OVERFLOW,
  CONVERSION_UNDERFLOW,
  CONVERSION_INVALID
} conversionStatus_t;

typedef enum {
  CMDPARSE_SUCCESS,
  CMDPARSE_NO_FILE,
  CMDPARSE_EXPECTED_VALUES,
  CMDPARSE_BAD_VALUES,
  CMDPARSE_MEMORY
} cmdParseStatus_t;

/* 
  Converts str (two comma-separated integers: ex. "123,123") into two integers
  and puts them in i1 and i2, respectively.
*/
conversionStatus_t handleSwitchIntInt(char* str, int& i1, int& i2);

/**
  Parse the command line args and fills references passed in.
*/
cmdParseStatus_t readCommandLineArgs(int argc, char** argv, bool& autOutput, 
  bool& dispHelp, bool& txGiven, bool& tyGiven, bool& wxGiven, bool& wyGiven, 
  bool& genGiven, int& numGenerations, int& txLow, int& txHigh, 
  int& tyLow, int& tyHigh, int& wxLow, int& wxHigh, int& wyLow, int& wyHigh, 
  std::string& inputFile);

/**
  Convert str to an int and put it in intConv. The resulting conversionStatus_t
  is returned.
*/
conversionStatus_t strToInt(int& intConv, const char* str);

/**
  Prints the error message in parseStatus. If parseStatus == CMDPARSE_SUCCESS, 
  then does nothing.
*/
void printCmdParseError(cmdParseStatus_t parseStatus);

/**
  Prints help screen for this program.
*/
void printHelp(char* cmd);

int main(int argc, char** argv) {
  bool autOutput, dispHelp, txGiven, tyGiven, wxGiven, wyGiven, genGiven;
  int numGenerations;
  int txLow, txHigh, tyLow, tyHigh, wxLow, wxHigh, wyLow, wyHigh;
  std::string inputFile;
  cmdParseStatus_t cmdStatus;
  cmdStatus = readCommandLineArgs(argc, argv, autOutput, dispHelp, txGiven, 
    tyGiven, wxGiven, wyGiven, genGiven, numGenerations, txLow, txHigh, tyLow, 
    tyHigh, wxLow, wxHigh, wyLow, wyHigh, inputFile);
  if(cmdStatus != CMDPARSE_SUCCESS) {
    printCmdParseError(cmdStatus);
    exit(1);
  }
  if(dispHelp) {
    printHelp(argv[0]);
    exit(0);
  }
  Point wbl(txLow, tyLow);
  Point wtr(txHigh, tyHigh);
  if(!genGiven) {
    numGenerations = 0;
  }
  if(wxGiven) {
    wbl.setX(wxLow);
    wtr.setX(wxHigh);
  }
  if(wyGiven) {
    wbl.setY(wyLow);
    wtr.setY(wyHigh);
  }
  Point bl(txLow, tyLow);
  Point tr(txHigh, tyHigh);
  GameGrid gg(Rect(bl, tr), Rect(wbl, wtr));
  gg.setSquare(Point(0,0), true);
  gg.setSquare(Point(0,1), true);
  gg.setSquare(Point(1,0), true);
  gg.setSquare(Point(-1,-1), true);
  gg.printToFile(std::cout, false);
}

cmdParseStatus_t readCommandLineArgs(int argc, char** argv, bool& autOutput, 
  bool& dispHelp, bool& txGiven, bool& tyGiven, bool& wxGiven, bool& wyGiven, 
  bool& genGiven, int& numGenerations, int& txLow, int& txHigh, 
  int& tyLow, int& tyHigh, int& wxLow, int& wxHigh, int& wyLow, int& wyHigh, 
  std::string& inputFile) {
  conversionStatus_t convStatus;
  autOutput = dispHelp = txGiven = tyGiven = wxGiven = wyGiven = genGiven = false;
  inputFile = "";
  numGenerations = 0;
  for(int i = 1; i < argc; i++) {
    if(argv[i][0] == '-') {
      if(argv[i][1] == 'a') {
        autOutput = true;
      }
      else if(argv[i][1] == 'h') {
        dispHelp = true;
      }
      else if(argv[i][1] == 'g') {
        if(argc <= i + 1) {
          return CMDPARSE_EXPECTED_VALUES;
        }
        convStatus = strToInt(numGenerations, argv[i+1]);
        if(convStatus != CONVERSION_SUCCESS) {
          return CMDPARSE_BAD_VALUES;
        }
        genGiven = true;
        ++i; /* don't parse argv[i+1] again */
      }
      else if(strcmp(argv[i], "-tx") == 0 || strcmp(argv[i], "-ty") == 0 
          || strcmp(argv[i], "-wx") == 0 || strcmp(argv[i], "-wy") == 0) {
        int *lowPtr, *highPtr;
        if(argc <= i + 1) {
          return CMDPARSE_EXPECTED_VALUES;
        }
        if(strcmp(argv[i], "-tx") == 0) {
          lowPtr = &txLow;
          highPtr = &txHigh;
          txGiven = true;
        }
        else if(strcmp(argv[i], "-ty") == 0) {
          lowPtr = &tyLow;
          highPtr = &tyHigh;
          tyGiven = true;
        }
        else if(strcmp(argv[i], "-wx") == 0) {
          lowPtr = &wxLow;
          highPtr = &wxHigh;
          wxGiven = true;
        }
        else if(strcmp(argv[i], "-wy") == 0) {
          lowPtr = &wyLow;
          highPtr = &wyHigh;
          wyGiven = true;
        }
        convStatus = handleSwitchIntInt(argv[i+1], *lowPtr, *highPtr);
        if(convStatus != CONVERSION_SUCCESS) {
          return CMDPARSE_BAD_VALUES;
        }
        /* don't parse argv[i+1] again */
        i+=1;
      }
    }
    else {
      inputFile = std::string(argv[i]);
    }
  }
  return CMDPARSE_SUCCESS;
}

conversionStatus_t handleSwitchIntInt(char* str, int& i1, int&i2) {
  conversionStatus_t convStat;
  char *iStr; 
  iStr = strtok(str, ",");
  convStat = strToInt(i1, iStr);
  if(convStat != CONVERSION_SUCCESS) {
    return convStat;
  }
  iStr = strtok(NULL, "");
  convStat = strToInt(i2, iStr);
  return convStat;
} 

conversionStatus_t strToInt(int& intConv, const char* string) {
  char* afterNum;
  long longConv;
  errno = 0;
  longConv = strtol(string, &afterNum, 10);
  if( (errno == ERANGE && longConv == LONG_MAX) || longConv > INT_MAX) {
    return CONVERSION_OVERFLOW;
  }
  else if( (errno == ERANGE && longConv == LONG_MIN) || longConv < INT_MIN) {
    return CONVERSION_UNDERFLOW;
  }
  else if(string[0] == '\0' || afterNum[0] != '\0') {
    return CONVERSION_INVALID;
  }
  intConv = longConv;
  return CONVERSION_SUCCESS;
}

void printCmdParseError(cmdParseStatus_t parseStatus) {
  if(parseStatus == CMDPARSE_SUCCESS) {
    return;
  }
  else if(parseStatus == CMDPARSE_NO_FILE) {
    std::cerr << "Input file not found in command line args. (-h for usage) \n";
  }
  else if(parseStatus == CMDPARSE_EXPECTED_VALUES) {
    std::cerr << "One or more values were not provided for certain switches (ex. "; 
    std::cerr << "-tx expects xLow\n"; 
    std::cerr << "and xHigh, but only xLow was given)\n";
  }
  else if(parseStatus == CMDPARSE_BAD_VALUES) {
    std::cerr << "One or more values could not be converted to integers\n";
  }
  else if(parseStatus == CMDPARSE_MEMORY) {
    std::cerr << "Memory allocation failed\n";
  }
}

void printHelp(char* cmd) {
  std::cout << "\nShowgen reads an aut file, simulates a given number of ";
  std::cout << "generations (defaults to 0),\nand prints the result\n\n";
  std::cout << "Usage: " << cmd << " [options] file.aut\n";
  std::cout << "\n";
  std::cout << "Options:\n";
  std::cout << "-a\t\toutput as aut file (otherwise ASCII display)\n";
  std::cout << "-g n\t\tsimulate n generations\n";
  std::cout << "-tx l,h\t\tset x range of terrain (overrides aut bounds)\n";
  std::cout << "-ty l,h\t\tset y range of terrain (overrides aut bounds)\n";
  std::cout << "-wx l,h\t\tset x range for output window (defaults to terrain x bounds)\n";
  std::cout << "-wy l,h\t\tset y range for output window (defaults to terrain y bounds\n";
  std::cout << "\n";
}
