#ifndef ACCUSATIONDIALOGCLASS_H_
#define ACCUSATIONDIALOGCLASS_H_

#include <QtGui>
#include "ui_AccusationDialogClass.h"
#include "SuggestionClass.h"

class AccusationDialogClass : public QDialog,
    private Ui::accusationDialogWindow
{
  Q_OBJECT

  public:
    AccusationDialogClass(SuggestionClass *accusationPtr, QWidget *parent = 0,
        Qt::WindowFlags f = 0);

  private:
    SuggestionClass *playerAccPtr;

  public slots:
    void setAccusation()
    {
      *playerAccPtr = SuggestionClass(suspectCombo->currentIndex(),
          weaponCombo->currentIndex() + NUMBER_OF_SUSPECTS,
          roomCombo->currentIndex() + NUMBER_OF_SUSPECTS + NUMBER_OF_WEAPONS);
    }

};

#endif
