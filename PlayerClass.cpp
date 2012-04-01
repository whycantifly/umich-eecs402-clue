#include <list>

#include "PlayerClass.h"
#include "enums.h"
#include "BoardLocationClass.h"
#include "ExceptionClass.h"

using namespace std;

PlayerClass::PlayerClass(QString name, SuspectEnum suspect, bool aiValue,
    bool gameHostValue)
{
  playerName = name;
  character = suspect;
  aiFlag = aiValue;
  hostFlag = gameHostValue;
  movedBySuggestion = false;
  setStartingLocation();
}

void PlayerClass::addCardToHand(CardEnum cardToAdd)
{
  hand.push_front(cardToAdd);
}

void PlayerClass::setStartingLocation()
{
  setPlayerLocation(STARTING_LOCATIONS[int(character)]);
}





//DUMMY AI CODE/////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void moveAi()
{

}

CardEnum PlayerClass::handleSuggestionAi(SuggestionClass suggestion)
{
  list<CardEnum>::iterator cardIter = hand.begin();
  bool foundMatch = false;
  CardEnum matchingCard;

  while(foundMatch == false)
  {
    if(suggestion == *cardIter)
    {
      matchingCard = *cardIter;
      foundMatch = true;
    }
    cardIter++;
  }
  return matchingCard;
}

void PlayerClass::handlePrerollAi()
{

}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
