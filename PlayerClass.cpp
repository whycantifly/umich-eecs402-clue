#include <list>
#include <queue>
#include <set>

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

//Moves the player over one tile; throws an exception if the move is
//outside the bounds of the board.
void PlayerClass::move(const QImage &currentBoard, const DirectionEnum &direction)
{
  BoardLocationClass newLocation = currentLocation;
  set<BoardLocationClass>::iterator visitedLocationIter;

  newLocation.move(currentBoard, direction);

//  if(locationsThisTurn.find(newLocation) != locationsThisTurn.end())
//  {
//    throw(ExceptionClass("You have already visited that tile this turn."));
//  }

  if(newLocation.getTileType(CLUE_BOARD_IMAGE) == ROOM_TILE)
  {
    for(visitedLocationIter = locationsThisTurn.begin(); visitedLocationIter !=
        locationsThisTurn.end(); visitedLocationIter++)
    {
      if(visitedLocationIter->getTileType(CLUE_BOARD_IMAGE) == ROOM_TILE &&
          visitedLocationIter->getRoom() == newLocation.getRoom())
      {
        throw(ExceptionClass("You have already visited that room this turn."));
      }
    }
  }
  currentLocation.move(currentBoard, direction);
}

void PlayerClass::addCardToHand(CardEnum cardToAdd)
{
  hand.insert(cardToAdd);
}

void PlayerClass::setStartingLocation()
{
  setPlayerLocation(STARTING_LOCATIONS[int(character)]);

  locationsThisTurn.insert(STARTING_LOCATIONS[character]);
}

void PlayerClass::addToDetectiveNotes(CardEnum card, SuspectEnum suspect)
{
  detectiveNotes[int(card)].second = suspect;
}

//DUMMY AI CODE/////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

CardEnum PlayerClass::handleSuggestionAi(SuggestionClass suggestion)
{
  set<CardEnum> cardMatches;
  set<CardEnum>::iterator cardMatchesIter;
  int randomIndex;

  CardEnum cardToAdd;


  if(hand.find(suspectToCard(suggestion.getSuspect())) != hand.end())
  {
    cardToAdd = *hand.find(suspectToCard(suggestion.getSuspect()));
    cardMatches.insert(cardToAdd);
  }
  if(hand.find(weaponToCard(suggestion.getWeapon())) != hand.end())
  {
    cardToAdd = *hand.find(weaponToCard(suggestion.getWeapon()));
    cardMatches.insert(cardToAdd);
  }
  if(hand.find(roomToCard(suggestion.getRoom())) != hand.end())
  {
    cardToAdd = *hand.find(roomToCard(suggestion.getRoom()));
    cardMatches.insert(cardToAdd);
  }

  randomIndex = rand() % cardMatches.size();

  cardMatchesIter = cardMatches.begin();
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
    SuggestionClass &aiSuggestion, queue<DirectionEnum> &aiMoves,
    int &aiExitDoorNumber)
{
  int targetDoorIndex;
  int startingDoorIndex = 0;

  if(currentLocation.checkPlayerBlocked(currentBoard) == false)
  {
    if(currentLocation.getTileType(currentBoard) == ROOM_TILE)
    {
      for(int i = 0; i < int(currentLocation.getRoom()); i++)
      {
        startingDoorIndex += NUMBER_OF_DOORS[i];
      }

      aiExitDoorNumber = rand() % NUMBER_OF_DOORS[currentLocation.getRoom()] +
          startingDoorIndex;
      targetDoorIndex = rand() % (TOTAL_NUMBER_OF_DOORS);
      aiMoves = DOOR_LOCATIONS[aiExitDoorNumber].getMovesToDoor(currentBoard,
          movesLeftThisTurn, targetDoorIndex);
      return MOVE;
    }
    else
    {
      targetDoorIndex = currentLocation.getClosestDoorIndex();
      aiMoves = currentLocation.getMovesToDoor(currentBoard, movesLeftThisTurn,
          targetDoorIndex);
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
  RoomEnum room = RoomEnum(rand() % NUMBER_OF_ROOMS);

  return SuggestionClass(suspect, weapon, room);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
