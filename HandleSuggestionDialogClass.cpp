//Name:       Elaine Pang, Taylor Tappe, Colin Zarzycki
//Date:       04/13/2012
//Assignment: EECS402 Project 5
//Purpose:    HandleSuggestionDialogClass member function definitions

#include <QtGui>
#include <set>

#include "HandleSuggestionDialogClass.h"
#include "enums.h"
#include "ClueMainWindowClass.h"
#include "SuggestionDialogClass.h"
#include "getCard.h"

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
  map<CardEnum, set<SuspectEnum> >::iterator handIter;

  suggPtr = suggestionPtr;
  cardToRevealPtr = cardPtr;

  suggestionInfo->setText(CARD_VALUES[getCard(currentPlayerChar)]
      + " suggested that the crime was committed in the " +
      CARD_VALUES[getCard(suggestionPtr->getRoom())] + " by " +
      CARD_VALUES[getCard(suggestionPtr->getSuspect())] + " with the "
      + CARD_VALUES[getCard(suggestionPtr->getWeapon())] + ".");

  for(handIter = hand.begin(); handIter != hand.end(); handIter++)
  {
    if(*suggestionPtr == handIter->first)
    {
      cardChoices.insert(pair<int, CardEnum>(cardSelectionBox->count(),
          handIter->first));
      cardSelectionBox->insertItem(hand.size(), CARD_VALUES[handIter->first]);
    }
  }
}

void HandleSuggestionDialogClass::showCard()
{
  *cardToRevealPtr = cardChoices.find(cardSelectionBox->currentIndex())->second;

  accept();
}
