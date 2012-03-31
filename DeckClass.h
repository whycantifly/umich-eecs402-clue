#ifndef _DECKCLASS_H_
#define _DECKCLASS_H_

#include <list>
#include "constants.h"

class DeckClass : private std::list<CardEnum>
{
  public:
    //Constructors

    //Constructor; initializes the deck so that it includes all values of
    //CardEnum and sets numberOfCards to the number of cards in the deck.
    DeckClass();

    //Function Prototypes

    //Gets a random card from the deck.
    CardEnum drawRandomCard();

    //Gets the value of a random card from the deck.
    CardEnum getRandomCard();

    //Removes the card with the indicated value from the deck.
    void removeCard(CardEnum cardToRemove);

    //Resets the deck so that it includes all values of CardEnum.
    void resetDeck();

    //Inline Functions

    //Returns the deck size
    int getDeckSize() const
    {
      return size();
    }
};

#endif
