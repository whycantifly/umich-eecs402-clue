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
