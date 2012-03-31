#ifndef _DECKCLASS_H__
#define _DECKCLASS_H_

#include <list>
#include "constants.h"

class DeckClass
{
  private:
    std::list<CardEnum> deck;     //Card deck
    int numberOfCards;            //Number of cards remaining in the deck

  public:
    //Constructors

    //Constructor; initializes the deck so that it includes all values of
    //CardEnum and sets numberOfCards to the number of cards in the deck.
    DeckClass()
    {
      //Code
    }

    //Function Prototypes

    //Gets a random card from the deck.
    void getRandomCard();

    //Resets the deck so that it includes all values of CardEnum.
    void resetDeck();

    //Inline Functions

    //Adds a card to the top of the deck.
    void addCard(CardEnum cardToAdd)
    {
      deck.push_front(cardToAdd);
      numberOfCards++;
    }
};

#endif
