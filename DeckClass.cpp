#include <stdlib.h>

#include "DeckClass.h"
#include "enums.h"

DeckClass::DeckClass()
{
  resetDeck();
}

CardEnum DeckClass::drawRandomCard()
{
  CardEnum card = getRandomCard();

  remove(card);
  return card;
}

CardEnum DeckClass::getRandomCard()
{
  std::list<CardEnum>::iterator cardIter = begin();
  CardEnum randomCard;
  int cardIndex = rand() % size();

  for(int i = 0; i < cardIndex; i++)
  {
    cardIter++;
  }

  randomCard = *cardIter;
  return randomCard;
}

void DeckClass::removeCard(CardEnum cardToRemove)
{
  remove(cardToRemove);
}

void DeckClass::resetDeck()
{
  clear();
  for(int i = 0; i < NUMBER_OF_SUSPECTS + NUMBER_OF_WEAPONS + NUMBER_OF_ROOMS;
      i++)
  {
    push_back(CardEnum(i));
  }
}
