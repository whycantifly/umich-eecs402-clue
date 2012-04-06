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
#include "getStartingDoorIndex.h"
#include "getDoorsForRoom.h"

using namespace std;

PlayerClass::PlayerClass(const bool aiValue, const bool gameHostValue,
    const BoardLocationClass &startingLocation, const DifficultyEnum
    aiDiff)
{
  aiFlag = aiValue;
  hostFlag = gameHostValue;
  movedSinceLastTurnFlag = false;
  dieRollThisTurn = 0;
  movesLeftThisTurn = 0;
  enteredRoomThisMoveFlag = false;
  currentLocation = startingLocation;
  aiDifficulty = aiDiff;
  endTurnFlag = false;
  resetLocationsThisTurn();

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

void PlayerClass::addToDetectiveNotes(CardEnum card, SuspectEnum suspect)
{
  detectiveNotes[int(card)].second = suspect;
}

set<BoardLocationClass> PlayerClass::getValidExitDoors(const QImage
    &currentBoard)
{
  //Variable Declarations
  set<BoardLocationClass> doors = getDoorsForRoom(currentLocation.getRoom());
  set<BoardLocationClass>::iterator doorIter = doors.begin();
  DirectionEnum direction;
  bool validDoorFlag;

  while(doorIter != doors.end())
  {
    validDoorFlag = false;
    direction = UP;
    while(direction < NUMBER_OF_DIRECTIONS && validDoorFlag == false)
    {
      if(doorIter->getTileInDir(direction).getTileType(currentBoard) ==
          UNOCCUPIED_TILE)
      {
        validDoorFlag = true;
      }
      direction = DirectionEnum(int(direction) + 1);
    }
    if(validDoorFlag == false)
    {
      doors.erase(doorIter);
    }
    doorIter++;
  }

  return doors;
}

set<DirectionEnum> PlayerClass::getValidMoveDirections(const QImage
    &currentBoard)
{
  //Variable Declarations
  set<DirectionEnum> validDirections;
  set<BoardLocationClass>::iterator visitedTilesIter;
  bool visitedRoomFlag = false;
  RoomEnum visitedRoom;

  //Find out if the player visited any rooms this turn
  visitedTilesIter = locationsThisTurn.begin();
  while(visitedTilesIter != locationsThisTurn.end() && visitedRoomFlag == false)
  {
    if(visitedTilesIter->getTileType(CLUE_BOARD_IMAGE) == ROOM_TILE)
    {
      visitedRoomFlag = true;
      visitedRoom = visitedTilesIter->getRoom();
    }
    visitedTilesIter++;
  }

  //Check each direction to see if its a feasible direction
  for(DirectionEnum i = UP; i <= RIGHT; i = DirectionEnum(int(i) + 1))
  {
    if(currentLocation.getTileInDir(i).getTileType(currentBoard) ==
        UNOCCUPIED_TILE)
    {
      validDirections.insert(i);
    }
    else if(currentLocation.getTileInDir(i).getTileType(currentBoard) ==
        ROOM_TILE)
    {
      try
      {
        currentLocation.getDoorIndex();
        if((visitedRoomFlag == false || currentLocation.getTileInDir(i).
            getRoom() != visitedRoom))
        {
          validDirections.insert(i);
        }
      }
      catch(ExceptionClass notADoor)
      {
        //Tile is not a door
      }
    }
  }

  return validDirections;
}

//DUMMY AI CODE/////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
BoardLocationClass PlayerClass::getAiTargetDoor()
{
  multimap<int, BoardLocationClass> targetDoorList =
      currentLocation.getTargetDoors();
  multimap<int, BoardLocationClass>::iterator doorIter = targetDoorList.begin();
  bool gotTargetFlag = false;
  int randomNumber;
  BoardLocationClass target;

  switch(aiDifficulty)
  {
    case EASY:
      do
      {
        randomNumber = rand() % AI_EASY_TARGET_RAND_MAX;
        if(randomNumber == 0)
        {
          gotTargetFlag = true;
        }
        else
        {
          doorIter++;
          if(doorIter == targetDoorList.end())
          {
            doorIter = targetDoorList.begin();
          }
        }
      }
      while(gotTargetFlag == false);
      target = doorIter->second;
      break;

    case MEDIUM:
      break;

    default:
      break;
  }
  return target;
}

BoardLocationClass PlayerClass::getAiExitDoor(QImage &currentBoard)
{
  //Variable Declarations
  set<BoardLocationClass> doors = getValidExitDoors(currentBoard);
  set<BoardLocationClass>::iterator doorIter = doors.begin();
  int doorPosition = rand() % doors.size();

  for(int i = 0; i < doorPosition; i++)
  {
    doorIter++;
  }

  return *doorIter;
}

DirectionEnum PlayerClass::getAiMove(QImage &currentBoard, BoardLocationClass
    &target)
{
  //Variable Declarations
  OrientationEnum moveOrientation;
  bool validMoveFlag = false;
  queue<DirectionEnum> moveDirection;
  set<DirectionEnum> validDirections = getValidMoveDirections(currentBoard);

  if(validDirections.empty() == true)
  {
    throw(ExceptionClass("No valid move directions"));
  }

  if(target == currentLocation && validDirections.find(
      DOOR_DIRECTIONS[target.getDoorIndex()]) != validDirections.end())
  {
    moveDirection.push(DOOR_DIRECTIONS[target.getDoorIndex()]);
  }
  else
  {
    if(target.getXCoord() == currentLocation.getXCoord())
    {
      moveOrientation = VERTICAL;
    }
    else if(target.getYCoord() == currentLocation.getYCoord())
    {
      moveOrientation = HORIZONTAL;
    }
    else
    {
      moveOrientation = OrientationEnum(rand() % 2);
    }

    while(moveDirection.size() < NUMBER_OF_DIRECTIONS / 2)
    {
      switch(moveOrientation)
      {
        case HORIZONTAL:
          if(target.getXCoord() < currentLocation.getXCoord() || (target.
              getXCoord() == currentLocation.getXCoord() && currentLocation.
              getXCoord() >= BOARD_WIDTH / 2))
          {
            moveDirection.push(LEFT);
          }
          else
          {
            moveDirection.push(RIGHT);
          }
          moveOrientation = VERTICAL;
          break;
        case VERTICAL:
          if(target.getYCoord() < currentLocation.getYCoord() || (target.
              getYCoord() == currentLocation.getYCoord() && currentLocation.
              getYCoord() >= BOARD_WIDTH / 2))
          {
            moveDirection.push(UP);
          }
          else
          {
            moveDirection.push(DOWN);
          }
          moveOrientation = HORIZONTAL;
          break;
      }
    }

    while(moveDirection.size() < NUMBER_OF_DIRECTIONS * 3 / 2)
    {
      switch(moveOrientation)
      {
        case HORIZONTAL:
          moveDirection.push(LEFT);
          moveDirection.push(RIGHT);
          moveOrientation = VERTICAL;
          break;
        case VERTICAL:
          moveDirection.push(UP);
          moveDirection.push(DOWN);
          moveOrientation = HORIZONTAL;
          break;
      }
    }

    while(validMoveFlag == false)
    {
      if(validDirections.find(moveDirection.front()) != validDirections.end())
      {
        validMoveFlag = true;
      }
      else
      {
        moveDirection.pop();
      }
    }
  }

  return moveDirection.front();
}

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

AiActionEnum PlayerClass::makeCornerRoomDecision()
{
  int randomNumber = rand() % 10;

  if(randomNumber < 3)
  {
    return USE_SECRET_PASSAGE;
  }
  else
  {
    return ROLL;
  }

}

AiActionEnum PlayerClass::handlePrerollAi(const QImage &currentBoard)
{
  //Make a suggestion if possible
  if(movedSinceLastTurnFlag == true)
  {
    return SUGGEST;
  }
  try
  {
    makeAiAccusation();
    return ACCUSE;
  }
  catch(ExceptionClass notReadyToAccuse)
  {
    //Roll if player is not in a room and there are valid moves
    if(currentLocation.getTileType(CLUE_BOARD_IMAGE) == UNOCCUPIED_TILE &&
        getValidMoveDirections(currentBoard).empty() == false)
    {
      return ROLL;
    }
    //Use secret passage in corner room otherwise
    else if(currentLocation.checkCornerRoom() == true)
    {
      return makeCornerRoomDecision();
    }
    //Roll if player is in a room and there are valid doors
    else if(currentLocation.getTileType(CLUE_BOARD_IMAGE) == ROOM_TILE &&
        getValidExitDoors(currentBoard).empty() == false)
    {
      return ROLL;
    }
    //Otherwise end turn
    else
    {
      return END_TURN;
    }
  }
}

AiActionEnum PlayerClass::handleAfterRollAi()
{
  try
  {
    makeAiAccusation();
    return ACCUSE;
  }
  catch(ExceptionClass notReadToAccuse)
  {
    return MOVE;
  }
}

SuggestionClass PlayerClass::makeAiSuggestion() const
{
  SuspectEnum suspect;
  WeaponEnum weapon;
  RoomEnum room;

  switch(aiDifficulty)
  {
    case EASY:
      suspect = SuspectEnum(rand() % NUMBER_OF_SUSPECTS);
      weapon = WeaponEnum(rand() % NUMBER_OF_WEAPONS);
      break;
    default:
      break;
  }

  room = currentLocation.getRoom();

  return SuggestionClass(suspect, weapon, room);
}

SuggestionClass PlayerClass::makeAiAccusation() const
{
  bool readyToAccuseFlag = false;
  SuggestionClass accusation;

  switch(aiDifficulty)
  {
    case EASY:
      break;
  }

  if(readyToAccuseFlag == false)
  {
    throw(ExceptionClass("Ai not ready to accuse"));
  }

  return accusation;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
