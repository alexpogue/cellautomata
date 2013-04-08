#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE AutPreprocessor test
#include <boost/test/unit_test.hpp>
#include <string>
#include <vector>
#include "AutPreprocessor.h"

void checkStringArrayMatches(std::string model, std::string* arr, int size) {
  for(int i = 0; i < size; i++) {
    BOOST_CHECK_EQUAL(model, arr[i]);
  }
}

BOOST_AUTO_TEST_CASE(spcSemicolon) {
  std::string spacesBeforeSemiExpected = "hi;";
  std::string spaceBeforeSemi = AutPreprocessor::preprocess("hi ;");
  std::string spacesBeforeSemi = AutPreprocessor::preprocess("hi  ;");
  std::string newlinesBeforeSemi = AutPreprocessor::preprocess("hi\n\n;");
  std::string crsBeforeSemi = AutPreprocessor::preprocess("hi\r\r;");
  std::string tabsBeforeSemi = AutPreprocessor::preprocess("hi\t\t;");
  std::string comboWhiteBeforeSemi = AutPreprocessor::preprocess("hi \t\r\n;");
  std::string beforeSemiStrs[] = {spaceBeforeSemi, spacesBeforeSemi, newlinesBeforeSemi, crsBeforeSemi, crsBeforeSemi, tabsBeforeSemi, comboWhiteBeforeSemi};
  checkStringArrayMatches(spacesBeforeSemiExpected, beforeSemiStrs, 6);
}

BOOST_AUTO_TEST_CASE(spcCurly) {
  std::string spacesBeforeCurlyExpected = "hi{";
  std::string spaceBeforeCurly = AutPreprocessor::preprocess("hi {");
  std::string spacesBeforeCurly = AutPreprocessor::preprocess("hi  {");
  std::string newlinesBeforeCurly = AutPreprocessor::preprocess("hi\n\n{");
  std::string crsBeforeCurly = AutPreprocessor::preprocess("hi\r\r{");
  std::string tabsBeforeCurly = AutPreprocessor::preprocess("hi\t\t{");
  std::string comboWhiteBeforeCurly= AutPreprocessor::preprocess("hi \t\r\n{");
  std::string spacesBeforeCurlyStrs[] = {spaceBeforeCurly, spacesBeforeCurly, newlinesBeforeCurly, crsBeforeCurly, tabsBeforeCurly, comboWhiteBeforeCurly};
  checkStringArrayMatches(spacesBeforeCurlyExpected, spacesBeforeCurlyStrs, 6);
}

BOOST_AUTO_TEST_CASE(spcInKeyword) {
  std::string whitespaceInKeywordExpected = "hi hi;";
  std::string spaceInKeyword = AutPreprocessor::preprocess("hi hi;");
  std::string spacesInKeyword = AutPreprocessor::preprocess("hi  hi;");
  std::string newlinesInKeyword = AutPreprocessor::preprocess("hi\n\nhi;");
  std::string crsInKeyword = AutPreprocessor::preprocess("hi\r\rhi;");
  std::string tabsInKeyword = AutPreprocessor::preprocess("hi\t\thi;");
  std::string comboWhiteInKeyword = AutPreprocessor::preprocess("hi \t\r\nhi;");
  std::string whitespaceInKeywordStrs[] = {spaceInKeyword, spacesInKeyword, newlinesInKeyword, crsInKeyword, tabsInKeyword, comboWhiteInKeyword};
  checkStringArrayMatches(whitespaceInKeywordExpected, whitespaceInKeywordStrs, 6);
}

BOOST_AUTO_TEST_CASE(spcOutsideKeyword) {
  std::string whitespaceOutsideKeywordExpected = "hi;hi;";
  std::string spaceOutsideKeyword = AutPreprocessor::preprocess("hi; hi; ");
  std::string spacesOutsideKeyword = AutPreprocessor::preprocess("hi;  hi;  ");
  std::string newlinesOutsideKeyword = AutPreprocessor::preprocess("hi;\n\nhi;\n\n");
  std::string crsOutsideKeyword = AutPreprocessor::preprocess("hi;\r\rhi;\r\r");
  std::string tabsOutsideKeyword = AutPreprocessor::preprocess("hi;\t\thi;\t\t");
  std::string comboWhiteOutsideKeyword = AutPreprocessor::preprocess("hi; \t\r\nhi;\t\r\n");
  std::string whitespaceOutsideKeywordStrs[] = {spaceOutsideKeyword, spacesOutsideKeyword, newlinesOutsideKeyword, crsOutsideKeyword, tabsOutsideKeyword, comboWhiteOutsideKeyword};
  checkStringArrayMatches(whitespaceOutsideKeywordExpected, whitespaceOutsideKeywordStrs, 6);
}

BOOST_AUTO_TEST_CASE(comments) {
  std::string commentsExpected = "hi;";
  std::string commentWithSemi = AutPreprocessor::preprocess("hi;#;invisible;");
  std::string commentWithCurly = AutPreprocessor::preprocess("hi;#{invisible;");
  std::string commentWithComment = AutPreprocessor::preprocess("hi;##invisible");
  std::string commentStrs[] = {commentWithSemi, commentWithCurly, commentWithComment};
  checkStringArrayMatches(commentsExpected, commentStrs, 3);
}
