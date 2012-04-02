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
      int test1 = suspectCombo->currentIndex();
      int test2 = weaponCombo->currentIndex();
      int test3 = roomCombo->currentIndex();
      *playerSugPtr = SuggestionClass(suspectCombo->currentIndex(),
          weaponCombo->currentIndex(),
          roomCombo->currentIndex());
    }
};

#endif
