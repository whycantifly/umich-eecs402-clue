#include <QApplication>
#include <time.h>
#include <stdlib.h>

#include "ClueMainWindowClass.h"

#include "ServerSocket.h"
#include "PackageClass.h"
#include "ClientSocket.h"
#include "SocketException.h"
#include <iostream>



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

