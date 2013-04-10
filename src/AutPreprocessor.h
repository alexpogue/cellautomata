#ifndef AUT_PREPROCESSOR_H
#define AUT_PREPROCESSOR_H

#include <string>

class AutPreprocessor {
  public:
    /** 
    Preprocesses rawAutText string such that the returned string is rawAutText
    with all extra spaces (defined below) and comments removed. Any whitespace 
    between keywords is replaced with a single space.

    Extra spaces refers to any whitespace not within a keyword, or all spaces
    within a keyword after the first space. For example, the extra spaces are  
    obviated in the string: "  Hello   world ; "
                             ^^      ^^     ^ ^
    Resulting in the output: "Hello world;"
     */
    static std::string preprocess(std::string rawAutText);
};

#endif
