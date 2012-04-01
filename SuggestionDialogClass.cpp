#include <QtGui>
#include "ClueMainWindowClass.h"
#include "SuggestionDialogClass.h"

SuggestionDialogClass::SuggestionDialogClass(SuggestionClass *suggestionPtr,
    ClueMainWindowClass *parent, Qt::WindowFlags f)
{
  setupUi(this);

  roomCombo->setCurrentIndex(int(parent->getCurrentPlayerRoom()) -
      NUMBER_OF_SUSPECTS - NUMBER_OF_WEAPONS);
  roomCombo->setEnabled(false);

  playerSugPtr = suggestionPtr;

  //Signals/slots
  connect(buttonBox, SIGNAL(accepted()), this, SLOT(setSuggestion()));
}
