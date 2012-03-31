#include "CaseFileClass.h"
#include "constants.h"
#include "enums.h"

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

    if(int(randomCard) < NUMBER_OF_SUSPECTS && haveSuspectFlag == false)
    {
      suspectCard = randomCard;
      deck.removeCard(randomCard);
      haveSuspectFlag = true;
    }
    else if(int(randomCard) < NUMBER_OF_SUSPECTS + NUMBER_OF_WEAPONS &&
        int(randomCard) >= NUMBER_OF_SUSPECTS && haveWeaponFlag == false)
    {
      weaponCard = randomCard;
      deck.removeCard(randomCard);
      haveWeaponFlag = true;
    }
    else if(int(randomCard) < NUMBER_OF_SUSPECTS + NUMBER_OF_WEAPONS +
        NUMBER_OF_ROOMS && int(randomCard) >= NUMBER_OF_SUSPECTS +
        NUMBER_OF_WEAPONS && haveRoomFlag == false)
    {
      roomCard = randomCard;
      deck.removeCard(randomCard);
      haveRoomFlag = true;
    }
  }
}
