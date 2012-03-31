#include <stdlib.h>

#include "DeckClass.h"
#include "enums.h"

DeckClass::DeckClass()
{
  resetDeck();
}

CardEnum DeckClass::drawRandomCard()
{
  removeCard(getRandomCard());
}

CardEnum DeckClass::getRandomCard()
{
  std::list<CardEnum>::iterator cardIter = deck.begin();
  CardEnum randomCard;
  int cardIndex = rand() % deck.size();

  for(int i = 0; i < cardIndex; i++)
  {
    cardIter++;
  }

  randomCard = *cardIter;
  return randomCard;
}

void DeckClass::removeCard(CardEnum cardToRemove)
{
  deck.remove(cardToRemove);
}

void DeckClass::resetDeck()
{
  deck.clear();
  for(int i = 0; i < NUMBER_OF_SUSPECTS + NUMBER_OF_WEAPONS + NUMBER_OF_ROOMS;
      i++)
  {
    deck.push_back(CardEnum(i));
  }
}
