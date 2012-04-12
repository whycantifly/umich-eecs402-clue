#include <list>

#include "SuggestionClass.h"
#include "enums.h"
#include "getCard.h"

using namespace std;

bool SuggestionClass::operator==(const map<CardEnum, set<SuspectEnum> > &playerHand)
    const
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

bool SuggestionClass::operator!=(const map<CardEnum, set<SuspectEnum> > &playerHand)
    const
{
  return !(*this == playerHand);
}

bool SuggestionClass::operator==(const CardEnum card) const
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

bool SuggestionClass::operator!=(const CardEnum card) const
{
  return !(*this == card);
}
