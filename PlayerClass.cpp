#include "PlayerClass.h"
#include "enums.h"
#include "BoardLocationClass.h"
#include "ExceptionClass.h"

PlayerClass::PlayerClass(QString name, CardEnum suspect, bool aiValue,
    bool gameHostValue)
{
  playerName = name;
  character = suspect;
  aiFlag = aiValue;
  gameHostValue = gameHost;
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

void PlayerClass::moveAi()
{
  //Code
}
