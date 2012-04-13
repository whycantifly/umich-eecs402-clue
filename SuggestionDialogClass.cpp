//Name:       Elaine Pang, Taylor Tappe, Colin Zarzycki
//Date:       04/13/2012
//Assignment: EECS402 Project 5
//Purpose:    SuggestionDialogClass member function definitions

#include <QtGui>
#include "ClueMainWindowClass.h"
#include "SuggestionDialogClass.h"

SuggestionDialogClass::SuggestionDialogClass(SuggestionClass *suggestionPtr,
    ClueMainWindowClass *parent, Qt::WindowFlags f) : QDialog(parent, f)
{
  setupUi(this);

  roomCombo->setCurrentIndex(int(parent->getCurrentPlayerRoom()));
  roomCombo->setEnabled(false);

  playerSugPtr = suggestionPtr;

  //Signals/slots
  connect(buttonBox, SIGNAL(accepted()), this, SLOT(setSuggestion()));
}
