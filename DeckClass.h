//Name:       Elaine Pang, Taylor Tappe, Colin Zarzycki
//Date:       04/13/2012
//Assignment: EECS402 Project 5
//Purpose:    DeckClass class definition

#ifndef _DECKCLASS_H_
#define _DECKCLASS_H_

#include <set>
#include "constants.h"
#include "SuggestionClass.h"

class DeckClass : private std::set<CardEnum>
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

    //Creates a case file from the deck and removes those cards from the
    //deck.
    SuggestionClass createCaseFile();

    //Inline Functions

    //Returns the deck size
    int getDeckSize() const
    {
      return size();
    }
};



#endif
