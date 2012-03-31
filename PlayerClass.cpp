#include "PlayerClass.h"
#include "enums.h"
#include "BoardLocationClass.h"
#include "ExceptionClass.h"

PlayerClass::PlayerClass(QString name, CardEnum suspect, bool aiValue,
    bool gameHostValue)
{
  playerName = name;
  character = suspect;
  ai = aiValue;
  gameHostValue = gameHost;
  activePlayer = true;
  setStartingLocation();
}

void PlayerClass::addCardToHand(CardEnum cardToAdd)
{
  //Code
}

void PlayerClass::setStartingLocation()
{
  setPlayerLocation(STARTING_LOCATIONS[int(character)]);
}

void PlayerClass::moveAi()
{
  //Code
}
