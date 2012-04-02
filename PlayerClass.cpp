#include <list>
#include <queue>

#include "PlayerClass.h"
#include "enums.h"
#include "BoardLocationClass.h"
#include "ExceptionClass.h"
#include "roomToCard.h"
#include "suspectToCard.h"
#include "weaponToCard.h"

using namespace std;

PlayerClass::PlayerClass(QString name, SuspectEnum suspect, bool aiValue,
    bool gameHostValue)
{

  playerName = name;
  character = suspect;
  aiFlag = aiValue;
  hostFlag = gameHostValue;
  movedSinceLastTurnFlag = false;
  dieRollThisTurn = 0;
  movesLeftThisTurn = 0;
  enteredRoomThisMoveFlag = false;
  setStartingLocation();

  for(int i = 0; i < NUMBER_OF_CARDS; i++)
  {
//    if(hand.find())
    detectiveNotes[i] = make_pair(CardEnum(i), UNKNOWN);
  }
}

void PlayerClass::addCardToHand(CardEnum cardToAdd)
{
  hand.insert(cardToAdd);
}

void PlayerClass::setStartingLocation()
{
  setPlayerLocation(STARTING_LOCATIONS[int(character)]);
}

void PlayerClass::addToDetectiveNotes(CardEnum card, SuspectEnum suspect)
{
  detectiveNotes[int(card)].second = suspect;
}

//DUMMY AI CODE/////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void PlayerClass::moveAi()
{

}

CardEnum PlayerClass::handleSuggestionAi(SuggestionClass suggestion)
{
  set<CardEnum> cardMatches;
  set<CardEnum>::iterator cardMatchesIter = cardMatches.begin();
  int randomIndex;

  cardMatches.insert(*hand.find(suspectToCard(suggestion.getSuspect())));
  cardMatches.insert(*hand.find(weaponToCard(suggestion.getWeapon())));
  cardMatches.insert(*hand.find(roomToCard(suggestion.getRoom())));

  randomIndex = rand() % cardMatches.size();
  for(int i = 0; i < randomIndex; i++)
  {
    cardMatchesIter++;
  }
  return *cardMatchesIter;
}

AiActionEnum PlayerClass::handlePrerollAi(const QImage &currentBoard, SuggestionClass &aiSuggestion)
{
  //Make a suggestion if possible
  if(movedSinceLastTurnFlag == true)
  {
    aiSuggestion = makeSuggestionAi();
    return SUGGEST;
  }
  //Roll if not blocked
  else if(currentLocation.checkPlayerBlocked(currentBoard) == false)
  {
    return ROLL;
  }
  //Use secret passage in corner room otherwise
  else if(currentLocation.checkCornerRoom() == true)
  {
    return USE_SECRET_PASSAGE;
  }
  //Otherwise end turn
  else
  {
    return END_TURN;
  }
}

AiActionEnum PlayerClass::handleAfterRollAi(const QImage &currentBoard,
    SuggestionClass &aiSuggestion, queue<DirectionEnum> &aiMoves, int &aiExitDoorNumber)
{
  BoardLocationClass targetLoc;

  if(currentLocation.checkPlayerBlocked(currentBoard) == false)
  {
    if(currentLocation.getTileType(currentBoard) == ROOM_TILE)
    {
      aiExitDoorNumber = rand() % NUMBER_OF_DOORS[currentLocation.getRoom()];
      targetLoc = DOOR_LOCATIONS[rand() & (TOTAL_NUMBER_OF_DOORS - 1)];
      aiMoves = targetLoc.getMovesTo(currentBoard, movesLeftThisTurn, targetLoc);
      return MOVE;
    }
    else
    {
      targetLoc = currentLocation.getClosestDoor();
      aiMoves = targetLoc.getMovesTo(currentBoard, movesLeftThisTurn, targetLoc);
      return MOVE;
    }
  }
  else
  {
    return END_TURN;
  }
}

SuggestionClass PlayerClass::makeSuggestionAi()
{
  SuspectEnum suspect = SuspectEnum(rand() % NUMBER_OF_SUSPECTS);
  WeaponEnum weapon = WeaponEnum(rand() % NUMBER_OF_WEAPONS);
  RoomEnum room = RoomEnum(rand() & NUMBER_OF_ROOMS);

  return SuggestionClass(suspect, weapon, room);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
