//Name:       Elaine Pang, Taylor Tappe, Colin Zarzycki
//Date:       04/13/2012
//Assignment: EECS402 Project 5
//Purpose:    main file for Clue game

#include <QApplication>
#include <time.h>
#include <stdlib.h>

#include "ClueMainWindowClass.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  
  ClueMainWindowClass *clueGame = new ClueMainWindowClass;

  srand(time(0));

  clueGame->setWindowTitle("Clue");
  clueGame->setWindowFlags(Qt::CustomizeWindowHint |
      Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);

  clueGame->show();

  return app.exec();

  delete clueGame;
}

