#include <QtGui>
#include "ClueMainWindowClass.h"
#include "SuggestionDialogClass.h"

SuggestionDialogClass::SuggestionDialogClass(SuggestionClass *suggestionPtr,
    ClueMainWindowClass *parent, Qt::WindowFlags f)
{
  setupUi(this);

  roomCombo->setCurrentIndex(int(parent->getCurrentPlayerRoom()));
  roomCombo->setEnabled(false);

  playerSugPtr = suggestionPtr;

  //Signals/slots
  connect(buttonBox, SIGNAL(accepted()), this, SLOT(setSuggestion()));
}
