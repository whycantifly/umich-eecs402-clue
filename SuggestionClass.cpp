#include <list>

#include "SuggestionClass.h"
#include "enums.h"
#include "getCard.h"

using namespace std;

bool SuggestionClass::operator==(set<CardEnum> &playerHand) const
{
  if(playerHand.find(getCard(suspect)) == playerHand.end() &&
      playerHand.find(getCard(weapon)) == playerHand.end() &&
      playerHand.find(getCard(room)) == playerHand.end())
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
    if(card == getCard(suspect) || card == getCard(weapon) ||
       card == getCard(room))
    {
      return true;
    }
    else
    {
      return false;
    }
  }
  catch(ExceptionClass newException)
  {

  }
}

bool SuggestionClass::operator!=(CardEnum card) const
{
  return !(*this == card);
}
