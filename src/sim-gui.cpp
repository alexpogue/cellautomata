#include <QApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>
#include <QTimer>
#include <stdexcept>
#include <cerrno>
#include <fstream>
#include "GridView.h"
#include "ControlDialog.h"
#include "GameGrid.h"
#include "AutParser.h"
#include "AutPreprocessor.h"

void readCommandLineArgs(int, char**, int&, int&, int&, int&, std::string&, bool&, bool&, bool&);
void stringToIntPair(char*, int&, int&);
void strToInt(int& intConv, const char* string);
void displayHelp(char* cmd);

int main(int argc, char** argv) {
  QApplication app(argc, argv);

  int txLow, txHigh, tyLow, tyHigh;
  bool dispHelp, txGiven, tyGiven;
  std::string autFilename;
  txLow = txHigh = tyLow = tyHigh = 0;
  dispHelp = txGiven = tyGiven = false;
  try {
    readCommandLineArgs(argc, argv, txLow, txHigh, tyLow, tyHigh, autFilename, dispHelp, txGiven, tyGiven);
  }
  catch(std::runtime_error e) {
    std::cerr << e.what() << "\n";
    exit(1);
  }
  if(dispHelp) {
    displayHelp(argv[0]);
  }
  Rect terrainBounds(Point(txLow, tyLow), Point(txHigh, tyHigh));
  GameGrid gg;
  try {
    AutParser::parse(autFilename, gg);
  }
  catch(std::ifstream::failure e) {
    std::cerr << "Could not open file: " << autFilename << "\n";
    exit(1);
  }
  std::cout << "state 0's rgb = (" << (int)gg.getGameStates()[0].getColor().red << ", ";
  std::cout << (int)gg.getGameStates()[0].getColor().blue << ", ";
  std::cout << (int)gg.getGameStates()[0].getColor().green << ")\n";
  std::cout << "state 1's rgb = (" << (int)gg.getGameStates()[1].getColor().red << ", ";
  std::cout << (int)gg.getGameStates()[1].getColor().blue << ", ";
  std::cout << (int)gg.getGameStates()[1].getColor().green << ")\n";
  Point tr;
  Point bl;
  bl.setX(gg.getTerrainBounds().getBottomLeft().getX());
  bl.setY(gg.getTerrainBounds().getBottomLeft().getY());
  tr.setX(gg.getTerrainBounds().getTopRight().getX());
  tr.setY(gg.getTerrainBounds().getTopRight().getY());

  if(txGiven) {
    bl.setX(txLow);
    tr.setX(txHigh);
  }
  if(tyGiven) {
    bl.setY(tyLow);
    tr.setY(tyHigh);
  }

  gg.setTerrainBounds(Rect(bl, tr));

  GridView* gridView = new GridView(gg);
  ControlDialog* control = new ControlDialog;
  QObject::connect(control, SIGNAL(dialogClosed()), gridView, SLOT(close()));
  QObject::connect(gridView, SIGNAL(gridClosed()), control, SLOT(close()));

  QTimer *timer = new QTimer;
  timer->setInterval(100);
  QObject::connect(timer, SIGNAL(timeout()), gridView, SLOT(displayNextGen()));
  QObject::connect(control, SIGNAL(changeDelay(int)), timer, SLOT(start(int)));
  QObject::connect(control, SIGNAL(pauseAnimation()), timer, SLOT(stop()));
  QObject::connect(control, SIGNAL(playAnimation()), timer, SLOT(start()));
  QObject::connect(control, SIGNAL(changeZoom(int)), gridView, SLOT(changeZoom(int)));
  QObject::connect(control, SIGNAL(stepGeneration()), gridView, SLOT(displayNextGen()));

  gridView->show();
  control->show();
  return app.exec();
}

void readCommandLineArgs(int argc, char** argv, int& txLow, int& txHigh, int& tyLow, int& tyHigh, std::string& fileIn, bool& dispHelp, bool& txGiven, bool& tyGiven) {
  if(argc <= 1) {
    throw std::runtime_error("error: not enough command line arguments (-h for usage)");
  }
  for(int i = 1; i < argc; i++) {
    if(strncmp(argv[i], "-h", 3) == 0) {
      dispHelp = true;
      return;
    }
    if(strncmp(argv[i], "-tx", 4) == 0 || strncmp(argv[i], "-ty", 4) == 0) {
      if(argc <= i + 1) {
        throw std::runtime_error(std::string("error: no values given for flag: ") + argv[i]);
      }
      if(argv[i][2] == 'x') {
        txGiven = true;
        stringToIntPair(argv[i+1], txLow, txHigh);
      }
      else if(argv[i][2] == 'y') {
        tyGiven = true;
        stringToIntPair(argv[i+1], tyLow, tyHigh);
      }
      /* skip int pair */
      ++i;
    }
    else {
      fileIn = argv[i];
    }
  }
}

void stringToIntPair(char* str, int& first, int& second) {
  char* secondStr = strchr(str, ',');
  if(secondStr == NULL || *(secondStr+1) == 0) {
    throw std::runtime_error(std::string("error: not enough values for terrain: expected two-integer range int,int (given: ") + str + ")");
  }
  *secondStr = 0;
  ++secondStr; // increment to after the comma
  strToInt(first, str);
  strToInt(second, secondStr);
}

void strToInt(int& intConv, const char* string) {
  char* afterNum;
  long longConv;
  errno = 0;
  longConv = strtol(string, &afterNum, 10);
  if( (errno == ERANGE && longConv == LONG_MAX) || longConv > INT_MAX) {
    throw std::runtime_error(std::string("error: overflow in integer conversion attempt: '") + string + "'");
  }
  else if( (errno == ERANGE && longConv == LONG_MIN) || longConv < INT_MIN) {
    throw std::runtime_error(std::string("error: underflow in integer conversion attempt: '") + string + "'");
  }
  else if(string[0] == '\0' || afterNum[0] != '\0') {
    throw std::runtime_error(std::string("error: invalid integer: '") + string + "'");
  }
  intConv = longConv;
}

void displayHelp(char* cmd) {
  std::cout << "sim-gui Help:\n";
  std::cout << "Description:\n";
  std::cout << "sim-gui provides a visualization (graphical user interface) of various cellular automata simulations\n\n";
  std::cout << "Usage: " << cmd << " [options] inputFile.aut\n\n";
  std::cout << "Options:\n";
  std::cout << "-h \t\tDisplay this help screen\n";
  std::cout << "-tx l,h\t\tSet the x-axis terrain range to [l, h] (inclusive)\n";
  std::cout << "-ty l,h\t\tSet the y-axis terrain range to [l, h] (inclusive)\n";
}
