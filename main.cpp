#include <QApplication>
#include <time.h>
#include <stdlib.h>

#include "ClueMainWindowClass.h"



int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  ClueMainWindowClass *clueGame = new ClueMainWindowClass;

  srand(time(0));

  clueGame->show();

  return app.exec();

  delete clueGame;
}

