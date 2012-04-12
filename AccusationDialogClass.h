#ifndef ACCUSATIONDIALOGCLASS_H_
#define ACCUSATIONDIALOGCLASS_H_

#include <QtGui>
#include "ui_AccusationDialogClass.h"
#include "SuggestionClass.h"

class AccusationDialogClass : public QDialog,
    private Ui::accusationDialogWindow
{
  Q_OBJECT

  private:
    SuggestionClass *playerAccPtr;        //Pointer to the player's accusation    
  
  public:
    //Constructor(SuggestionClass*, QWidget*, Qt::WindowFlags)
    AccusationDialogClass(SuggestionClass *accusationPtr, QWidget *parent = 0,
        Qt::WindowFlags f = 0);

  public slots:
    //Sets the accusation to the accusation entered by the player.
    void setAccusation()
    {
      *playerAccPtr = SuggestionClass(suspectCombo->currentIndex(),
          weaponCombo->currentIndex(),
          roomCombo->currentIndex());
    }
};

#endif
