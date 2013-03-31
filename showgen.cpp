#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <climits>
#include <cerrno>

using namespace std;

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
  Converts iStr1, which ends in a comma (ex. "123,") and iStr2, no comma (ex. 
  "456") to ints and places the result into i1 and i2. Returns the appropriate 
  conversionStatus_t.
*/
conversionStatus_t handleSwitchIntInt(char* str, int& i1, int& i2);

/**
  Parse the command line args for values passed in.
*/
cmdParseStatus_t readCommandLineArgs(int argc, char** argv, bool& autOutput, 
  bool& dispHelp, bool& txGiven, bool& tyGiven, bool& wxGiven, bool& wyGiven, 
  bool& genGiven, int& numGenerations, int& txLow, int& txHigh, 
  int& tyLow, int& tyHigh, int& wxLow, int& wxHigh, int& wyLow, int& wyHigh, 
  string& inputFile);

/**
  Convert str to an int and put it in intConv. The resulting conversionStatus_t
  is returned.
*/
conversionStatus_t strToInt(int& intConv, const char* str);

/**
  Takes the passed in cmdParseStatus_t and prints out any errors in it. If 
  cmdStatus == CMDPARSE_SUCCESS, then it returns without doing anything.
*/
void printCmdParseError(cmdParseStatus_t parseStatus);

int main(int argc, char** argv) {
  bool autOutput, dispHelp, txGiven, tyGiven, wxGiven, wyGiven, genGiven;
  int numGenerations;
  int txLow, txHigh, tyLow, tyHigh, wxLow, wxHigh, wyLow, wyHigh;
  string inputFile;
  cmdParseStatus_t cmdStatus;
  cmdStatus = readCommandLineArgs(argc, argv, autOutput, dispHelp, txGiven, 
    tyGiven, wxGiven, wyGiven, genGiven, numGenerations, txLow, txHigh, tyLow, 
    tyHigh, wxLow, wxHigh, wyLow, wyHigh, inputFile);
  if(cmdStatus != CMDPARSE_SUCCESS) {
    printCmdParseError(cmdStatus);
    exit(1);
  }
  if(autOutput) cout << "autOutput\n";
  if(dispHelp) cout << "dispHelp\n";
  if(txGiven) cout << "txLow = " << txLow << ", txHigh = " << txHigh << "\n";
  if(tyGiven) cout << "tyLow = " << tyLow << ", tyHigh = " << tyHigh << "\n";
  if(wxGiven) cout << "wxLow = " << wxLow << ", wxHigh = " << wxHigh << "\n";
  if(wyGiven) cout << "wyLow = " << wyLow << ", wyHigh = " << wyHigh << "\n";
  if(genGiven) cout << "gen = " << numGenerations << "\n";
  cout << "inputFile = " << inputFile << "\n";
}

cmdParseStatus_t readCommandLineArgs(int argc, char** argv, bool& autOutput, 
  bool& dispHelp, bool& txGiven, bool& tyGiven, bool& wxGiven, bool& wyGiven, 
  bool& genGiven, int& numGenerations, int& txLow, int& txHigh, 
  int& tyLow, int& tyHigh, int& wxLow, int& wxHigh, int& wyLow, int& wyHigh, 
  string& inputFile) {
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
        /* don't parse argv[i+1] again */
        ++i;
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
      inputFile = string(argv[i]);
    }
  }
  return CMDPARSE_SUCCESS;
}

conversionStatus_t handleSwitchIntInt(char* str, int& i1, int&i2) {
  conversionStatus_t convStat;
  char *iStr1, *iStr2; 
  iStr1 = strtok(str, ",");
  convStat = strToInt(i1, iStr1);
  if(convStat != CONVERSION_SUCCESS) {
    return convStat;
  }
  /* read until null terminator */
  iStr2 = strtok(NULL, "");
  convStat = strToInt(i2, iStr2);
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
    cout << "Input file not found in command line args. (-h for usage) \n";
  }
  else if(parseStatus == CMDPARSE_EXPECTED_VALUES) {
    cout << "One or more values were not provided for certain switches (ex. "; 
    cout << "-tx expects xLow\n"; 
    cout << "and xHigh, but only xLow was given)\n";
  }
  else if(parseStatus == CMDPARSE_BAD_VALUES) {
    cout << "One or more values could not be converted to integers\n";
  }
  else if(parseStatus == CMDPARSE_MEMORY) {
    cout << "Memory allocation failed\n";
  }
}
