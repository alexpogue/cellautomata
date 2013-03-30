#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
using namespace std;

void readCommandLineArgs(int argc, char** argv, bool& autOutput, bool& dispHelp, 
  bool& txGiven, bool& tyGiven, bool& wxGiven, bool& wyGiven, bool& genGiven,
  unsigned int& numGenerations, int& txLow, int& txHigh, int& tyLow, 
  int& tyHigh, int& wxLow, int& wxHigh, int& wyLow, int& wyHigh, 
  string inputFile);

int main(int argc, char** argv) {
  bool autOutput, dispHelp, txGiven, tyGiven, wxGiven, wyGiven, genGiven;
  unsigned int numGenerations;
  int txLow, txHigh, tyLow, tyHigh, wxLow, wxHigh, wyLow, wyHigh;
  string inputFile = "hi";
  readCommandLineArgs(argc, argv, autOutput, dispHelp, txGiven, tyGiven, 
    wxGiven, wyGiven, genGiven, numGenerations, txLow, txHigh, 
    tyLow, tyHigh, wxLow, wxHigh, wyLow, wyHigh, inputFile);
  cout << "dispHelp = " << dispHelp << "\n";
  cout << "autOutput = " << autOutput << "\n";
  cout << "genGiven = " << genGiven << "\n";
  cout << "numGenerations = " << numGenerations << "\n";
}

void readCommandLineArgs(int argc, char** argv, bool& autOutput, bool& dispHelp, 
  bool& txGiven, bool& tyGiven, bool& wxGiven, bool& wyGiven, bool& genGiven, 
  unsigned int& numGenerations, int& txLow, int& txHigh, int& tyLow, 
  int& tyHigh, int& wxLow, int& wxHigh, int& wyLow, int& wyHigh, 
  string inputFile) {
  char* afterNum;
  autOutput = dispHelp = txGiven = tyGiven = wxGiven = wyGiven = genGiven = false;
  numGenerations = 0;
  for(int i = 0; i < argc; i++) {
    if(argv[i][0] == '-') {
      if(argv[i][1] == 'a') {
        autOutput = true;
      }
      else if(argv[i][1] == 'h') {
        dispHelp = true;
      }
      else if(argv[i][1] == 'g') {
        numGenerations = strtoul(argv[i+1], &afterNum, 10);
        if(argv[i][0] != '\0' && afterNum[0] == '\0') {
          genGiven = true;
        }
        ++i;
      }
      /*
      else if(strcmp(argv[i], "-tx") == 0) {
        char* low;
        low = malloc(strlen(argv[i+1] - 1));
        if(!low) {
          return -1;
        }
        low = strncpy(argv[i+1], low, strlen(argv[i+1] - 1));
        txLow = strtol(low, &afterNum, 10);
        if(low[0] != '\0' && afterNum[0] == '\0') {
      */    
      /* TODO: CONTINUE THIS! */
          
        
    }
  }
}
