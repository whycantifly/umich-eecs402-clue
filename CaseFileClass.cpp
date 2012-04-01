#include "CaseFileClass.h"
#include "constants.h"
#include "enums.h"
#include "cardToSuspect.h"
#include "cardToWeapon.h"
#include "cardToRoom.h"

using namespace std;

CaseFileClass::CaseFileClass(DeckClass &deck)
{
//  createCaseFile(deck);
}

void CaseFileClass::createCaseFile(DeckClass &deck)
{
  //Variable Declarations
  bool haveSuspectFlag = false;
  bool haveWeaponFlag = false;
  bool haveRoomFlag = false;
  CardEnum randomCard;

  while(haveSuspectFlag == false || haveWeaponFlag == false || haveRoomFlag ==
      false)
  {
    randomCard = deck.getRandomCard();

    try
    {
      cardToSuspect(randomCard);
      if(haveSuspectFlag == false)
      {
        suspect = cardToSuspect(randomCard);
        deck.removeCard(randomCard);
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
          deck.removeCard(randomCard);
          haveWeaponFlag = true;
        }
      }
      catch(ExceptionClass notAWeapon)
      {
        if(haveRoomFlag == false)
        {
          room = cardToRoom(randomCard);
          deck.removeCard(randomCard);
          haveRoomFlag = true;
        }
      }
    }
  }
  int i = 0;
}
