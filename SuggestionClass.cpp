#include <list>

#include "SuggestionClass.h"
#include "enums.h"
#include "roomToCard.h"
#include "suspectToCard.h"
#include "weaponToCard.h"

using namespace std;

bool SuggestionClass::operator==(set<CardEnum> &playerHand) const
{
  if(playerHand.find(suspectToCard(suspect)) == playerHand.end() &&
      playerHand.find(weaponToCard(weapon)) == playerHand.end() &&
      playerHand.find(roomToCard(room)) == playerHand.end())
  {
    return false;
  }
  else
  {
    return true;
  }
}

bool SuggestionClass::operator!=(set<CardEnum> &playerHand) const
{
  return !(*this == playerHand);
}

bool SuggestionClass::operator==(CardEnum card) const
{
  try
  {
    if(card == suspectToCard(suspect) || card == weaponToCard(weapon) ||
       card == roomToCard(room))
    {
      return true;
    }
    else
    {
      return false;
    }
  }
  catch(ExceptionClass exception)
  {

  }
}

bool SuggestionClass::operator!=(CardEnum card) const
{
  return !(*this == card);
}
