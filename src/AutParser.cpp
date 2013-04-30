#include "AutParser.h"
#include "AutPreprocessor.h"
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdio>
#include "GameRules.h"

struct ParseData {
  std::vector<Point> aliveCells;
  std::vector<int> stateNums;
  Rect terrain;
  std::string name;
  std::string chars;
  GameRules rules;
  std::vector<StateColor> colors;
  bool rulesGiven;
};

enum Keyword {
  KEYWORD_XRANGE,
  KEYWORD_YRANGE,
  KEYWORD_INITIAL,
  KEYWORD_UNKNOWN,
  KEYWORD_NAME,
  KEYWORD_CHARS,
  KEYWORD_COLORS,
  KEYWORD_RULES
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
void handleColorsArgs(const std::string& kwStr, ParseData& pd);
void printInitialStatementWarning(const std::string& kwStr, size_t startPos, size_t endPos);
void handleInitialStatement(const std::string& statement, int& state, ParseData& pd);
void handleYStatement(const std::string& statement, int currState, ParseData& pd);
void handleStateStatement(const std::string& statement, int& state);
void printCouldNotFindExpected(std::string expected, std::string keyword);
size_t handleNextXVal(const std::string& kwStr, int yVal, int state, size_t pos, ParseData& pd);
void getNextStatement(const std::string& wholeText, size_t pos, std::string& statement);
bool strToIntExpectedBlock(const std::string& str, int& i, const std::string& expected, const std::string& block);
void assignRangeArgs(const std::string& args, int& low, int& high);
void handleRulesArg(const std::string& kwStr, ParseData& pd); 
GameRules convertToGameRules(const std::string& ruleStr);

void AutParser::parse(std::istream& file, GameGrid& gg) {
  std::string rawAutText;
  if(&file == &std::cin/* && !std::cin.eof()*/) {
    for(;;) {
      int readChar = file.get();
      if(readChar == EOF) break;
      rawAutText += readChar;
    }
  }
  else {
    std::stringstream ss;
    ss << file.rdbuf();
    rawAutText = ss.str();
  }
  std::string preprocessedText = AutPreprocessor::preprocess(rawAutText);
  ParseData pd;
  pd.rulesGiven = false;
  size_t autLength = preprocessedText.length();
  size_t pos = 0;
  while(pos < autLength) {
    pos = parseNextKeyword(preprocessedText, pos, pd);
  }
  gg.setTerrainBounds(pd.terrain);
  gg.setWindowBounds(pd.terrain);
  gg.resetGrid();
  for(unsigned int i = 0; i < pd.aliveCells.size(); i++) {
    gg.setSquare(pd.aliveCells[i], pd.stateNums[i]);
  }
  gg.setName(pd.name);
  gg.setGameStates(pd.chars);
  gg.setGameStateColors(pd.colors);
  /* TODO: consider using rules to ignore state assignments 
            (ex. Conway only needs 2 states) */
  if(pd.rulesGiven) {
    gg.setRules(pd.rules);
  }
  else {
    std::cerr << "Warning: no Rules declaration in file, defaulting to ConwaysLife\n";
    gg.setRules(RULES_CONWAYS_LIFE);
  }
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
  else if(k == KEYWORD_COLORS) {
    handleColorsArgs(keywordStr, pd);
  }
  else if(k == KEYWORD_RULES) {
    handleRulesArg(keywordStr, pd);
  }
  else if(k == KEYWORD_UNKNOWN) {
    std::cerr << "Warning: keyword \"" << keywordStr << "\" is unknown\n";
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
  else if(keywordStr.substr(0, 6) == "Colors") {
    return KEYWORD_COLORS;
  }
  else if(keywordStr.substr(0, 5) == "Rules") {
    return KEYWORD_RULES;
  }
  else {
    return KEYWORD_UNKNOWN;
  }
}

void handleInitialBlock(const std::string& kwStr, ParseData& pd) {
  size_t currPos = kwStr.find_first_of("{") + 1;
  size_t endBracketPos = kwStr.find_first_of("}");
  int state = 1;
  std::string statementStr; 
  while(currPos < endBracketPos) {
    getNextStatement(kwStr, currPos, statementStr);
    handleInitialStatement(statementStr, state, pd);
    currPos += statementStr.length();
  }
}

void getNextStatement(const std::string& wholeText, size_t pos, std::string& statement) {
  size_t semicolonPos = wholeText.find_first_of(";", pos);
  statement = wholeText.substr(pos, semicolonPos - pos + 1);
}

void handleInitialStatement(const std::string& statement, int& state, ParseData& pd) {
  size_t firstCharPos = statement.find_first_not_of(" \t\r\n");
  if(statement[firstCharPos] == 'Y') {
    handleYStatement(statement, state, pd);
  }
  else if(statement.substr(firstCharPos, 5) == "State") {
    handleStateStatement(statement, state);
  }
}

void handleYStatement(const std::string& statement, int currState, ParseData& pd) {
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
    currPos = handleNextXVal(statement, yVal, currState, currPos, pd);
  }
}

void handleStateStatement(const std::string& statement, int& state) {
  size_t numeralStart = statement.find_first_of("0123456789");
  size_t numeralEnd = statement.find_last_of("0123456789");
  std::string istr = statement.substr(numeralStart, numeralEnd - numeralStart + 1);
  state = atoi(istr.c_str());
}

size_t handleNextXVal(const std::string& kwStr, int yVal, int state, size_t pos, ParseData& pd) {
  size_t xValEnd = kwStr.find_first_of(",;", pos);
  std::string xValStr = kwStr.substr(pos, xValEnd - pos);
  int xVal;
  if(strToIntExpectedBlock(xValStr, xVal, "[valid xVal]", "initial") == false) {
    return xValEnd + 1;
  }
  pd.aliveCells.push_back(Point(xVal, yVal));
  pd.stateNums.push_back(state);
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
  std::cerr << "Warning: could not find expected identifier: `" << expected << "' in keyword: `" << keyword << "'\n";
}

void printInitialStatementWarning(const std::string& kwStr, size_t startPos, size_t endPos) {
    std::cerr << "Warning: unrecognized initial statement: " 
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
    std::cerr << "Warning, could not find range arg divider\n";
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
  size_t commaPos = kwStr.find_first_of(",;"); /* semicolon because there may be a single value given */
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

void handleColorsArgs(const std::string& kwStr, ParseData& pd) {
  size_t firstNumeral = 0;
  size_t commaPos = 0;
  while(commaPos != std::string::npos && firstNumeral != std::string::npos) {
    StateColor color;
    size_t startParen = kwStr.find_first_of("(", commaPos);
    firstNumeral = kwStr.find_first_of("0123456789", startParen);
    commaPos = kwStr.find_first_of(",;", firstNumeral);
    size_t endParen = kwStr.find_first_of(")", startParen);
    for(int j = 0; j < 3 && firstNumeral < endParen; j++) {
      /* so that we don't include end paran in numeralCharStr */
      if(endParen < commaPos) {
        commaPos = endParen;
      }
      std::string numeralCharStr = kwStr.substr(firstNumeral, commaPos - firstNumeral);
      unsigned char colorVal = atoi(numeralCharStr.c_str());
      if(j == 0) color.red = colorVal;
      else if(j == 1) color.green = colorVal;
      else if(j == 2) color.blue = colorVal;
      firstNumeral = kwStr.find_first_of("0123456789", commaPos);
      commaPos = kwStr.find_first_of(",;", commaPos + 1);
    }
    pd.colors.push_back(color);
  }
}

void handleRulesArg(const std::string& kwStr, ParseData& pd) {
  pd.rulesGiven = true;
  size_t firstLetter = kwStr.find_first_of(" ") + 1;
  size_t lastLetter = kwStr.length() - 1;
  pd.rules = convertToGameRules(kwStr.substr(firstLetter, lastLetter - firstLetter + 1)); 
}

GameRules convertToGameRules(const std::string& ruleStr) {
  if(ruleStr.substr(0, 11) == "ConwaysLife") {
    return RULES_CONWAYS_LIFE;
  }
  else if(ruleStr.substr(0, 11) == "BriansBrain") {
    return RULES_BRIANS_BRAIN;
  }
  else if(ruleStr.substr(0, 9) == "WireWorld") {
    return RULES_WIRE_WORLD;
  }
  else if(ruleStr.substr(0, 11) == "LangtonsAnt") {
    return RULES_LANGTONS_ANT;
  }
  else {
    std::cerr << "Unidentified rules: `" << ruleStr << "'. Defaulting to Conway's Life\n";
    return RULES_CONWAYS_LIFE;
  }
}
