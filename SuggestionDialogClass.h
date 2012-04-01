#ifndef _SUGGESTIONDIALOGCLASS_H_
#define _SUGGESTIONDIALOGCLASS_H_

#include <QtGui>
#include "ui_SuggestionDialogClass.h"
#include "SuggestionClass.h"
#include "ClueMainWindowClass.h"

class SuggestionDialogClass : public QDialog,
    private Ui::suggestionDialogWindow
{
  Q_OBJECT

  public:
    SuggestionDialogClass(SuggestionClass *suggestionPtr,
        ClueMainWindowClass *parent = 0, Qt::WindowFlags f = 0);

  private:
    SuggestionClass *playerSugPtr;

  public slots:
    void setSuggestion()
    {
      *playerSugPtr = SuggestionClass(suspectCombo->currentIndex(),
          weaponCombo->currentIndex() + NUMBER_OF_SUSPECTS,
          roomCombo->currentIndex() + NUMBER_OF_SUSPECTS + NUMBER_OF_WEAPONS);
    }
};

#endif
