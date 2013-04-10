#include "AutPreprocessor.h"
#include <string>
#include <iostream>

size_t getNextNonWhite(const std::string& autText, size_t pos);
size_t removeLeadingWhitespace(std::string& autText, size_t pos);
size_t preprocessKeyword(std::string& autText, size_t pos);
size_t removeLine(std::string& autText, size_t pos);
size_t getNextNewline(const std::string& autText, size_t pos);
size_t getNextKeywordEnding(const std::string& autText, size_t pos);
bool handleComment(std::string& autText, size_t pos);
std::string getWhitespaceChars();
bool isWhitespace(char possible);

std::string AutPreprocessor::preprocess(std::string rawAutText) {
  size_t pos = 0;
  for(;;){
    removeLeadingWhitespace(rawAutText, pos);
    if(pos > rawAutText.size()) {
      break;
    }
    if(handleComment(rawAutText, pos)) {
      // must remove whitespace again if we handled a comment
      continue;
    }
    if(pos > rawAutText.size()) {
      break;
    }
    pos = preprocessKeyword(rawAutText, pos);
    if(pos > rawAutText.size()) {
      break;
    }
  }
  return rawAutText;
}

size_t removeLeadingWhitespace(std::string& autText, size_t pos) {
  size_t nonWhitePos = getNextNonWhite(autText, pos);
  if(nonWhitePos == std::string::npos) {
    nonWhitePos = autText.size();
  }
  size_t whiteSpaceLength = nonWhitePos - pos;
  autText.erase(pos, whiteSpaceLength);
  return whiteSpaceLength;
}

size_t getNextNonWhite(const std::string& autText, size_t pos) {
  return autText.find_first_not_of(getWhitespaceChars(), pos);
}

std::string getWhitespaceChars() {
  return " \t\f\v\n\r";
}

bool handleComment(std::string& autText, size_t pos) {
  if(autText[pos] == '#') {
    removeLine(autText, pos);
    return true;
  }
  return false;
}

size_t preprocessKeyword(std::string& autText, size_t pos) {
  size_t keywordEndingPos = getNextKeywordEnding(autText, pos);
  if(keywordEndingPos == std::string::npos) {
    keywordEndingPos = autText.size();
  }
  for(; pos < keywordEndingPos; pos++) {
    if(autText[pos] == '#') {
      keywordEndingPos -= removeLine(autText, pos);
    }
    else if(isWhitespace(autText[pos])) {
      autText.replace(pos, 1, " ");
      keywordEndingPos -= removeLeadingWhitespace(autText, pos+1); 
    }
  }
  if(isWhitespace(autText[keywordEndingPos - 1])) {
    autText.erase(keywordEndingPos - 1, 1);
    --pos;
  }
  return pos + 1;
}

bool isWhitespace(char possible) {
  return getWhitespaceChars().find(std::string(1,possible)) != std::string::npos;
}

size_t removeLine(std::string& autText, size_t pos) {
  size_t charAfterNewlinePos = getNextNewline(autText, pos) + 1;
  if(charAfterNewlinePos == std::string::npos) {
    charAfterNewlinePos = autText.size();
  }
  size_t lineLength = charAfterNewlinePos - pos;
  autText.erase(pos, lineLength);
  return lineLength;
}

size_t getNextNewline(const std::string& autText, size_t pos) {
  return autText.find_first_of("\n", pos);
}

size_t getNextKeywordEnding(const std::string& autText, size_t pos) {
  return autText.find_first_of(";{", pos);
}
