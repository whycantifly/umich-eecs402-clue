#include <stdlib.h>

#include "DeckClass.h"
#include "enums.h"
#include "ExceptionClass.h"
#include "SuggestionClass.h"
#include "cardToSuspect.h"
#include "cardToWeapon.h"
#include "cardToRoom.h"

DeckClass::DeckClass()
{
  resetDeck();
}

CardEnum DeckClass::drawRandomCard()
{
  CardEnum card = getRandomCard();

  erase(card);
  return card;
}

CardEnum DeckClass::getRandomCard()
{
  std::set<CardEnum>::iterator cardIter = begin();
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
  erase(cardToRemove);
}

void DeckClass::resetDeck()
{
  clear();
  for(int i = 0; i < NUMBER_OF_CARDS; i++)
  {
    insert(CardEnum(i));
  }
}

SuggestionClass DeckClass::createCaseFile()
{
  //Variable Declarations
  SuspectEnum suspect;
  WeaponEnum weapon;
  RoomEnum room;
  bool haveSuspectFlag = false;
  bool haveWeaponFlag = false;
  bool haveRoomFlag = false;
  CardEnum randomCard;

  while(haveSuspectFlag == false || haveWeaponFlag == false || haveRoomFlag ==
      false)
  {
    randomCard = getRandomCard();

    try
    {
      cardToSuspect(randomCard);
      if(haveSuspectFlag == false)
      {
        suspect = cardToSuspect(randomCard);
        removeCard(randomCard);
        haveSuspectFlag = true;
      }
    }
    catch(ExceptionClass notASuspect)
    {
      try
      {
        cardToWeapon(randomCard);
        if(haveWeaponFlag == false)
        {
          weapon = cardToWeapon(randomCard);
          removeCard(randomCard);
          haveWeaponFlag = true;
        }
      }
      catch(ExceptionClass notAWeapon)
      {
        if(haveRoomFlag == false)
        {
          room = cardToRoom(randomCard);
          removeCard(randomCard);
          haveRoomFlag = true;
        }
      }
    }
  }
  return SuggestionClass(suspect, weapon, room);
}
