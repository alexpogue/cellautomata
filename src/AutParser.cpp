#include "AutParser.h"
#include "AutPreprocessor.h"
#include "GameGrid.h"
#include <iostream>

enum Keyword {
  KEYWORD_XRANGE,
  KEYWORD_YRANGE,
  KEYWORD_INITIAL,
  KEYWORD_UNKNOWN
};

size_t parseNextKeyword(const std::string& autText, size_t pos, GameGrid& grid);
size_t getKeywordEnding(const std::string& autText, size_t pos);
Keyword getKeywordFromString(const std::string& keywordStr);
void handleKeywordString(const std::string& keywordStr, GameGrid& grid);
void handleInitialBlock(const std::string& kwStr, GameGrid& grid);
void handleXrangeArgs(const std::string& kwStr, GameGrid& grid);
void handleYrangeArgs(const std::string& kwStr, GameGrid& grid);

void AutParser::parse(const std::string& rawAutText, GameGrid& gg) {
  std::string preprocessedText = AutPreprocessor::preprocess(rawAutText);
  size_t autLength = preprocessedText.length();
  size_t pos = 0;
  while(pos < autLength) {
    pos = parseNextKeyword(preprocessedText, pos, gg);
  }
  /* Not Implemented */
}

size_t parseNextKeyword(const std::string& autText, size_t pos, GameGrid& grid) {
  size_t keywordEnd = getKeywordEnding(autText, pos);
  if(keywordEnd == std::string::npos) {
    return std::string::npos;
  }
  std::string keywordStr = autText.substr(pos, keywordEnd - pos);
  handleKeywordString(keywordStr, grid);
  return keywordEnd + 1;
}

size_t getKeywordEnding(const std::string& autText, size_t pos) {
  size_t nextSemicolonPos = autText.find_first_of(";", pos);
  size_t nextOpenCurlyPos = autText.find_first_of("{", pos);
  if(nextOpenCurlyPos > nextSemicolonPos || nextOpenCurlyPos == std::string::npos) {
    return nextSemicolonPos;
  }
  else {
    return autText.find_first_of("}") + 1;
  }
}

void handleKeywordString(const std::string& keywordStr, GameGrid& grid) {
  Keyword k = getKeywordFromString(keywordStr);
  if(k == KEYWORD_INITIAL) {
    handleInitialBlock(keywordStr, grid);
  }
  else if(k == KEYWORD_XRANGE) {
    handleXrangeArgs(keywordStr, grid);
  }
  else if(k == KEYWORD_YRANGE) {
    handleYrangeArgs(keywordStr, grid);
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
  else {
    return KEYWORD_UNKNOWN;
  }
}

void handleInitialBlock(const std::string& kwStr, GameGrid& grid) {
  /* NOT IMPLEMENTED */
}

void handleXrangeArgs(const std::string& kwStr, GameGrid& grid) {
  /* NOT IMPLEMENTED */
}

void handleYrangeArgs(const std::string& kwStr, GameGrid& grid) {
  /* NOT IMPLEMENTED */
}
