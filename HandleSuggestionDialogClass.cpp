#include <QtGui>
#include <set>

#include "HandleSuggestionDialogClass.h"
#include "enums.h"
#include "ClueMainWindowClass.h"
#include "SuggestionDialogClass.h"
#include "roomToCard.h"
#include "suspectToCard.h"
#include "weaponToCard.h"

using namespace std;

HandleSuggestionDialogClass::HandleSuggestionDialogClass(
    ClueMainWindowClass *parent, Qt::WindowFlags f) : QDialog(parent, f)
{
  currentPlayerChar = parent->getCurrentPlayerSuspect();
  hand = parent->getThisPlayerHand();


  setupUi(this);

  //Signals/slots
  connect(okayButton, SIGNAL(clicked()), this, SLOT(showCard()));
}

void HandleSuggestionDialogClass::setupDialogBox(SuggestionClass *suggestionPtr,
    CardEnum *cardPtr)
{
  set<CardEnum>::iterator handIter;

  suggPtr = suggestionPtr;
  cardToRevealPtr = cardPtr;

  suggestionInfo->setText(CARD_VALUES[suspectToCard(currentPlayerChar)]
      + " suggested that the crime was committed in the " +
      CARD_VALUES[roomToCard(suggestionPtr->getRoom())] + " by " +
      CARD_VALUES[suspectToCard(suggestionPtr->getSuspect())] + " with the "
      + CARD_VALUES[weaponToCard(suggestionPtr->getWeapon())] + ".");

  for(handIter = hand.begin(); handIter != hand.end(); handIter++)
  {
    if(*suggestionPtr == *handIter)
    {
      cardChoices.insert(pair<int, CardEnum>(cardSelectionBox->count(), *
          handIter));
      cardSelectionBox->insertItem(hand.size(), CARD_VALUES[*handIter]);
    }
  }
}

void HandleSuggestionDialogClass::showCard()
{
  *cardToRevealPtr = cardChoices.find(cardSelectionBox->currentIndex())->second;

  accept();
}
