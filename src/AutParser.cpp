#include "AutParser.h"
#include "AutPreprocessor.h"
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <fstream>

struct ParseData {
  std::vector<Point> aliveCells;
  Rect terrain;
  std::string name;
  std::string chars;
  std::string rules;
  std::vector<std::vector<char> > colors;
};

enum Keyword {
  KEYWORD_XRANGE,
  KEYWORD_YRANGE,
  KEYWORD_INITIAL,
  KEYWORD_UNKNOWN,
  KEYWORD_NAME,
  KEYWORD_CHARS
};

size_t parseNextKeyword(const std::string& autText, size_t pos, ParseData& pd);
size_t getKeywordEnding(const std::string& autText, size_t pos);
Keyword getKeywordFromString(const std::string& keywordStr);
void handleKeywordString(const std::string& keywordStr, ParseData& pd);
void handleInitialBlock(const std::string& kwStr, ParseData& pd);
void handleXrangeArgs(const std::string& kwStr, ParseData& pd);
void handleYrangeArgs(const std::string& kwStr, ParseData& pd);
void handleNameArg(const std::string& kwStr, ParseData& pd);
void handleCharsArgs(const std::string& kwStr, ParseData& pd);
void printInitialStatementWarning(const std::string& kwStr, size_t startPos, size_t endPos);
void handleInitialStatement(const std::string& statement, ParseData& pd);
void printCouldNotFindExpected(std::string expected, std::string keyword);
size_t handleNextXVal(const std::string& kwStr, int yVal, size_t pos, ParseData& pd);
void getNextStatement(const std::string& wholeText, size_t pos, std::string& statement);
bool strToIntExpectedBlock(const std::string& str, int& i, const std::string& expected, const std::string& block);
void assignRangeArgs(const std::string& args, int& low, int& high);

void AutParser::parse(const std::string& fileName, GameGrid& gg) {
  std::ifstream file;
  file.exceptions(std::ios::failbit);
  try {
    file.open(fileName.c_str());
  } catch(std::ifstream::failure e) {
    throw;
  }
  std::stringstream ss;
  ss << file.rdbuf();
  std::string rawAutText(ss.str());
  std::string preprocessedText = AutPreprocessor::preprocess(rawAutText);
  ParseData pd;
  size_t autLength = preprocessedText.length();
  size_t pos = 0;
  while(pos < autLength) {
    pos = parseNextKeyword(preprocessedText, pos, pd);
  }
  gg.setTerrainBounds(pd.terrain);
  gg.setWindowBounds(pd.terrain);
  gg.resetGrid();
  for(unsigned int i = 0; i < pd.aliveCells.size(); i++) {
    gg.setSquare(pd.aliveCells[i], true);
  }
  gg.setName(pd.name);
  gg.setGameStates(pd.chars);
}

size_t parseNextKeyword(const std::string& autText, size_t pos, ParseData& pd) {
  size_t keywordEnd = getKeywordEnding(autText, pos);
  if(keywordEnd == std::string::npos) {
    return std::string::npos;
  }
  std::string keywordStr = autText.substr(pos, keywordEnd - pos);
  handleKeywordString(keywordStr, pd);
  return keywordEnd + 1;
}

size_t getKeywordEnding(const std::string& autText, size_t pos) {
  size_t nextSemicolonPos = autText.find_first_of(";", pos);
  size_t nextOpenCurlyPos = autText.find_first_of("{", pos);
  if(nextOpenCurlyPos > nextSemicolonPos || nextOpenCurlyPos == std::string::npos) {
    return nextSemicolonPos;
  }
  else {
    size_t nextEndBracePos = autText.find_first_of("}");
    return autText.find_first_of(";", nextEndBracePos);
  }
}

void handleKeywordString(const std::string& keywordStr, ParseData& pd) {
  Keyword k = getKeywordFromString(keywordStr);
  if(k == KEYWORD_INITIAL) {
    handleInitialBlock(keywordStr, pd);
  }
  else if(k == KEYWORD_XRANGE) {
    handleXrangeArgs(keywordStr, pd);
  }
  else if(k == KEYWORD_YRANGE) {
    handleYrangeArgs(keywordStr, pd);
  }
  else if(k == KEYWORD_NAME) {
    handleNameArg(keywordStr, pd);
  }
  else if(k == KEYWORD_CHARS) {
    handleCharsArgs(keywordStr, pd);
  }
  else if(k == KEYWORD_UNKNOWN) {
    std::cout << "Warning: keyword \"" << keywordStr << "\" is unknown\n";
  }
}

Keyword getKeywordFromString(const std::string& keywordStr) {
  if(keywordStr.substr(0, 7) == "Initial") {
    return KEYWORD_INITIAL;
  }
  else if(keywordStr.substr(0, 6) == "Xrange") {
    return KEYWORD_XRANGE;
  }
  else if(keywordStr.substr(0, 6) == "Yrange") {
    return KEYWORD_YRANGE;
  }
  else if(keywordStr.substr(0, 4) == "Name") {
    return KEYWORD_NAME;
  }
  else if(keywordStr.substr(0, 5) == "Chars") {
    return KEYWORD_CHARS;
  }
  else {
    return KEYWORD_UNKNOWN;
  }
}

void handleInitialBlock(const std::string& kwStr, ParseData& pd) {
  size_t currPos = kwStr.find_first_of("{") + 1;
  size_t endBracketPos = kwStr.find_first_of("}");
  std::string statementStr; 
  while(currPos < endBracketPos) {
    getNextStatement(kwStr, currPos, statementStr);
    handleInitialStatement(statementStr, pd);
    currPos += statementStr.length();
  }
}

void getNextStatement(const std::string& wholeText, size_t pos, std::string& statement) {
  size_t semicolonPos = wholeText.find_first_of(";", pos);
  statement = wholeText.substr(pos, semicolonPos - pos + 1);
}

void handleInitialStatement(const std::string& statement, ParseData& pd) {
  size_t equalsPos = statement.find_first_of("=");
  if(equalsPos == std::string::npos) {
    printCouldNotFindExpected("=", "initial");
    return;
  }
  size_t colonPos = statement.find_first_of(":");
  if(colonPos == std::string::npos) {
    printCouldNotFindExpected(":", "initial");
    return;
  }
  std::string yValStr = statement.substr(equalsPos + 1, colonPos - equalsPos - 1);
  int yVal;
  if(strToIntExpectedBlock(yValStr, yVal, "[valid yVal]", "initial") == false) {
    return;
  }
  size_t currPos = colonPos + 1;
  while(currPos < statement.length()) {
    currPos = handleNextXVal(statement, yVal, currPos, pd);
  }
}

size_t handleNextXVal(const std::string& kwStr, int yVal, size_t pos, ParseData& pd) {
  size_t xValEnd = kwStr.find_first_of(",;", pos);
  std::string xValStr = kwStr.substr(pos, xValEnd - pos);
  int xVal;
  if(strToIntExpectedBlock(xValStr, xVal, "[valid xVal]", "initial") == false) {
    return xValEnd + 1;
  }
  pd.aliveCells.push_back(Point(xVal, yVal));
  return xValEnd + 1;
}

bool strToIntExpectedBlock(const std::string& str, int& i, const std::string& expected, const std::string& block) {
  std::istringstream ss(str);
  ss >> i;
  if(ss.fail()) {
    printCouldNotFindExpected(expected, block);
    return false;
  }
  return true;
}

void printCouldNotFindExpected(std::string expected, std::string keyword) {
  std::cout << "Warning: could not find expected identifier: `" << expected << "' in keyword: `" << keyword << "'\n";
}

void printInitialStatementWarning(const std::string& kwStr, size_t startPos, size_t endPos) {
    std::cout << "Warning: unrecognized initial statement: " 
      << kwStr.substr(startPos, endPos - startPos + 1) << "\n";
}

void handleXrangeArgs(const std::string& kwStr, ParseData& pd) {
  int xLow, xHigh;
  size_t xLowPos = kwStr.find_first_of(" ") + 1;
  assignRangeArgs(kwStr.substr(xLowPos), xLow, xHigh);
  pd.terrain.setXLow(xLow);
  pd.terrain.setXHigh(xHigh);
}

void handleYrangeArgs(const std::string& kwStr, ParseData& pd) {
  int yLow, yHigh;
  size_t yLowPos = kwStr.find_first_of(" ") + 1;
  assignRangeArgs(kwStr.substr(yLowPos), yLow, yHigh);
  pd.terrain.setYLow(yLow);
  pd.terrain.setYHigh(yHigh);
}

void assignRangeArgs(const std::string& args, int& low, int& high) {
  size_t dividerPos = args.find_first_of(" ");
  if(dividerPos == std::string::npos) {
    std::cout << "Warning, could not find range arg divider\n";
    return;
  }
  std::string lowStr = args.substr(0, dividerPos);
  std::string highStr = args.substr(dividerPos + 1, args.length() - dividerPos - 1);
  if(strToIntExpectedBlock(lowStr, low, "[valid low value]", "range") == false) {
    return;
  }
  if(strToIntExpectedBlock(highStr, high, "[valid high value]", "range") == false) {
    return;
  }
}

void handleNameArg(const std::string& kwStr, ParseData& pd) {
  size_t nameStart = kwStr.find_first_of("\"") + 1;
  size_t secondQuotePos  = kwStr.find_first_of("\"", nameStart);
  pd.name = kwStr.substr(nameStart, secondQuotePos - nameStart);
}

void handleCharsArgs(const std::string& kwStr, ParseData& pd) {
  std::string chars;
  size_t commaPos = kwStr.find_first_of(",");
  size_t charStartPos = kwStr.find_last_of(" ", commaPos) + 1;
  while(commaPos != std::string::npos && charStartPos != std::string::npos) {
    std::string numeralCharStr = kwStr.substr(charStartPos, commaPos - charStartPos);
    char alphabetChar = atoi(numeralCharStr.c_str());
    chars += alphabetChar;
    charStartPos = kwStr.find_first_of("0123456789", commaPos);
    commaPos = kwStr.find_first_of(",;", charStartPos);
    if(commaPos == std::string::npos) {
      commaPos = kwStr.length();
    }
  }
  pd.chars = chars;
}
