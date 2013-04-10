#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE AutPreprocessor test
#include <boost/test/unit_test.hpp>
#include <string>
#include "AutPreprocessor.h"

void checkStringArrayMatches(std::string model, std::string* arr, int size) {
  for(int i = 0; i < size; i++) {
    BOOST_CHECK_EQUAL(model, arr[i]);
  }
}

BOOST_AUTO_TEST_CASE(cornerCases) {
  std::string singleSemicolon = AutPreprocessor::preprocess(";");
  std::string singleCurly = AutPreprocessor::preprocess("{");
  std::string empty = AutPreprocessor::preprocess("");
  std::string singleChar = AutPreprocessor::preprocess("c");
  std::string singleWhitespace = AutPreprocessor::preprocess(" ");
  BOOST_CHECK_EQUAL(";", singleSemicolon);
  BOOST_CHECK_EQUAL("{", singleCurly);
  BOOST_CHECK_EQUAL("", empty);
  BOOST_CHECK_EQUAL("c", singleChar);
  BOOST_CHECK_EQUAL("", singleWhitespace);
}

BOOST_AUTO_TEST_CASE(statements) {
  std::string lotsOfWhite = AutPreprocessor::preprocess("h  e\t  \n  l\r l \vo\fworld;\t\n\r\f\vs        ec    on  dke   ywo  rd;\t  \n");
  std::string lotsOfWhiteExpected = "h e l l o world;s ec on dke ywo rd;";
  std::string endCurlies = AutPreprocessor::preprocess("out{inner;inner;};next{nextinner;};");
  std::string endCurliesExpected = "out{inner;inner;};next{nextinner;};";
  std::string endCurliesWithSpaces = AutPreprocessor::preprocess("out  {  inner;  inner;  }  ;  next{nextinner  ;  }  ;  ");
  std::string curliesWithSpaces = AutPreprocessor::preprocess("  {   { {{  {");
  std::string curliesWithSpacesExpected = "{{{{{";
  std::string endWithoutKeywordEnd = AutPreprocessor::preprocess("hello      ");
  std::string endWithoutKeywordEndExpected = "hello";
  BOOST_CHECK_EQUAL(lotsOfWhiteExpected, lotsOfWhite);
  BOOST_CHECK_EQUAL(endCurlies, endCurliesExpected);
  BOOST_CHECK_EQUAL(endCurliesWithSpaces, endCurliesExpected);
  BOOST_CHECK_EQUAL(curliesWithSpacesExpected, curliesWithSpaces);
  BOOST_CHECK_EQUAL(endWithoutKeywordEndExpected, endWithoutKeywordEnd);
}

BOOST_AUTO_TEST_CASE(leadingSpace) {
  std::string leadingSpacesExpected = "hi;";
  std::string leadingSpace = AutPreprocessor::preprocess(" hi;");
  std::string leadingSpaces = AutPreprocessor::preprocess("  hi;");
  std::string leadingNewlines = AutPreprocessor::preprocess("\n\nhi;");
  std::string leadingCrs = AutPreprocessor::preprocess("\r\rhi;");
  std::string leadingTabs = AutPreprocessor::preprocess("\t\thi;");
  std::string leadingComboWhite = AutPreprocessor::preprocess(" \t\r\n hi;");
  std::string leadingSpacesStrs[] = {leadingSpace, leadingSpaces, leadingNewlines, leadingCrs, leadingTabs, leadingComboWhite};
  checkStringArrayMatches(leadingSpacesExpected, leadingSpacesStrs, 6);
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

BOOST_AUTO_TEST_CASE(noKeywordEnding) {
  std::string noKeywordEndingExpected = "hi";
  std::string trailingSpaces = AutPreprocessor::preprocess("hi \t\r\n");
  std::string leadingSpaces = AutPreprocessor::preprocess(" \t\r\nhi");
  std::string leadingAndTrailingSpaces = AutPreprocessor::preprocess(" \t\r\nhi \t\r\n");
  std::string noKeywordEndingStrs[] = {trailingSpaces, leadingSpaces, leadingAndTrailingSpaces};
  checkStringArrayMatches(noKeywordEndingExpected, noKeywordEndingStrs, 3); 
}

BOOST_AUTO_TEST_CASE(fullFile) {
  std::string fullFileExpected = "This bogus keyword should cause everything to be ignored until the next semicolon, allowing for future expansion;Another{more evil bogus keyword;we must also consume;the inner bogus statements;};Xrange -10 10;Yrange -5 5;Initial{Y = 2 : -1;Y=1: -2, -1;Y=-1: 2,3,4;};";
  std::string fullFile = "\
#\n\
# Comments, to be ignored\n\
#\n\
This bogus keyword should cause everything to be ignored\n\
until the next semicolon, allowing for future expansion ;\n\n\
\
Another { more evil bogus keyword; we must also consume;\
the inner bogus statements; };\n\
# Ugly formatting, but allowed:\n\
Xrange\n\
-10 10; Yrange -5 5;\n\n\
\
Initial {\n\
  Y = 2 :   -1;         # Sets (-1, 2) to \"alive\"\n\
  Y=1:      -2, -1;     # Sets (-2, 1) and (-1, 1) to \"alive\"\n\
  Y=-1:     2,3,4;      # Sets (2, -1), (3, -1), and (4, -1) to \"alive\"\n\
};";
  std::string fullFilePre = AutPreprocessor::preprocess(fullFile);
  BOOST_CHECK_EQUAL(fullFileExpected, fullFilePre); 
}

BOOST_AUTO_TEST_CASE(comments) {
  std::string commentsExpected = "hi;";
  std::string commentWithSemi = AutPreprocessor::preprocess("hi;#;invisible;");
  std::string commentWithCurly = AutPreprocessor::preprocess("hi;#{invisible;");
  std::string commentWithComment = AutPreprocessor::preprocess("hi;##invisible");
  std::string commentInsideKeyword = AutPreprocessor::preprocess("hi#comment\n;");
  std::string commentStrs[] = {commentWithSemi, commentWithCurly, commentWithComment};
  checkStringArrayMatches(commentsExpected, commentStrs, 3);
}
