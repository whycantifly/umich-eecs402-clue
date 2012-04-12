#include <stdlib.h>

#include "DeckClass.h"
#include "enums.h"
#include "ExceptionClass.h"
#include "SuggestionClass.h"
#include "getRoom.h"
#include "getSuspect.h"
#include "getWeapon.h"

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
  SuspectEnum suspect;              //Suspect in the case file
  WeaponEnum weapon;                //Weapon in the case file
  RoomEnum room;                    //Room in the case file
  bool haveSuspectFlag = false;     //True = have a suspect in the case file      
  bool haveWeaponFlag = false;      //True = have a weapon in the case file
  bool haveRoomFlag = false;        //True = have a room in the case file
  CardEnum randomCard;              //Random card chosen from the deck

  while(haveSuspectFlag == false || haveWeaponFlag == false || haveRoomFlag ==
      false)
  {
    randomCard = getRandomCard();

    try
    {
      getSuspect(randomCard);
      if(haveSuspectFlag == false)
      {
        suspect = getSuspect(randomCard);
        removeCard(randomCard);
        haveSuspectFlag = true;
      }
    }
    catch(ExceptionClass notASuspect)
    {
      try
      {
        getWeapon(randomCard);
        if(haveWeaponFlag == false)
        {
          weapon = getWeapon(randomCard);
          removeCard(randomCard);
          haveWeaponFlag = true;
        }
      }
      catch(ExceptionClass notAWeapon)
      {
        if(haveRoomFlag == false)
        {
          room = getRoom(randomCard);
          removeCard(randomCard);
          haveRoomFlag = true;
        }
      }
    }
  }
  return SuggestionClass(suspect, weapon, room);
}
