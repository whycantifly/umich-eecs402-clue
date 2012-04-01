#include <list>

#include "SuggestionClass.h"
#include "enums.h"
#include "cardToSuspect.h"
#include "cardToWeapon.h"
#include "cardToRoom.h"

using namespace std;

bool SuggestionClass::operator==(list<CardEnum> &playerHand) const
{
  list<CardEnum>::iterator currentCardIter = playerHand.begin();
  bool matchFlag = false;

  while(currentCardIter != playerHand.end() && matchFlag == false)
  {
    matchFlag = (*this == *currentCardIter);
  }
  return matchFlag;
}


bool SuggestionClass::operator==(const CardEnum &card) const
{
  bool matchFlag = false;

  try
  {
    if(suspect == cardToSuspect(card))
    {
      matchFlag = true;
    }
  }
  catch(ExceptionClass notASuspect)
  {
    try
    {
      if(weapon == cardToWeapon(card))
      {
        matchFlag = true;
      }
    }
    catch(ExceptionClass notAWeapon)
    {
      if(room == cardToRoom(card))
      {
        matchFlag = true;
      }
    }
  }
  return matchFlag;
}
