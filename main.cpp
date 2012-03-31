#include <QApplication>
#include <time.h>
#include <stdlib.h>

#include "ClueMainWindowClass.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  ClueMainWindowClass *dialog = new ClueMainWindowClass;

  srand(time(0));

  dialog->show();

  return app.exec();
}

