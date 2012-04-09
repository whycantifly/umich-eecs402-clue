#include <list>
#include <queue>
#include <set>

#include "PlayerClass.h"
#include "enums.h"
#include "BoardLocationClass.h"
#include "ExceptionClass.h"
#include "cardToSuspect.h"
#include "cardToWeapon.h"
#include "cardToRoom.h"
#include "roomToCard.h"
#include "suspectToCard.h"
#include "weaponToCard.h"
#include "getStartingDoorIndex.h"
#include "getDoorsForRoom.h"
#include "getCardType.h"

using namespace std;

PlayerClass::PlayerClass(const bool aiValue, const bool gameHostValue,
    const BoardLocationClass &startingLocation, const DifficultyEnum
    aiDiff)
{
  aiFlag = aiValue;
  hostFlag = gameHostValue;
  dieRollThisTurn = 0;
  movesLeftThisTurn = 0;
  currentLocation = startingLocation;
  aiDifficulty = aiDiff;
  lastAction = END_TURN;
  movedThisTurnFlag = false;
  gameOverFlag = false;
  resetLocationsThisTurn();

  for(int i = 0; i < NUMBER_OF_CARDS; i++)
  {
    detectiveNotes[i] = make_pair(CardEnum(i), UNKNOWN);
  }
}

// Prints contents of PlayerClass to a string for packet transfer
// over a network
string PlayerClass::printPlayerToString()
{
  int i;
   
  // Get number of cards in hand and set up iterator for set
  int numCardsInHand = (int) hand.size();
  set<CardEnum>::iterator cardIter;
  
  // Get number of locations this turn and set up iterator
  int numLocationsVisited = (int) locationsThisTurn.size();
  set<BoardLocationClass>::iterator numLocsIter;
      
//         BoardLocationClass currentLocation; //Current location on the board
//     bool hostFlag;                      //True = host (or local play);
//                                         //False = not host
//     bool aiFlag;                        //True = AI; false = human player
//     bool movedThisTurnFlag;             //True = moved this turn; false = not
//     ActionEnum lastAction;              //Last action taken by the player
//     std::set<CardEnum> hand;            //Cards in the players hand; ordered in
//                                         //order of CardEnum
//     int dieRollThisTurn;                //Value of the die roll this turn
//     int movesLeftThisTurn;              //Moves left to make this turn
//     std::set<BoardLocationClass> locationsThisTurn;
//                                         //List of locations player has visited
//                                         //this turn
//     std::pair<CardEnum, SuspectEnum> detectiveNotes[NUMBER_OF_CARDS];
//     DifficultyEnum aiDifficulty;
  
  // Open string stream
  stringstream ss;
  
  // Print boring single value stats
  ss << currentLocation.getXCoord() << " " << currentLocation.getYCoord() 
    << " " << hostFlag << " " << aiFlag << " " <<
    movedThisTurnFlag << " " << lastAction << " " 
    
    // Print number of cards in hand
    << numCardsInHand << " " ;
    
    // Print cards in this players hand
    for (cardIter = hand.begin();
      cardIter != hand.end();
      cardIter++)
      {
      ss << *cardIter << " ";
      }
    
    ss << dieRollThisTurn << " " << movesLeftThisTurn << " "
    
    // Print number of locations visited
    << numLocationsVisited << " ";
    
    // Print Locations this turn
    for (numLocsIter = locationsThisTurn.begin();
      numLocsIter != locationsThisTurn.end();
      numLocsIter++)
      {
      ss << (*numLocsIter).getXCoord() << " " << (*numLocsIter).getYCoord() << " ";
      }
    
    // Print detective cards
    for (i = 0;
      i < NUMBER_OF_CARDS;
      i++)
      {
      ss << detectiveNotes[i].first << " " << detectiveNotes[i].second << " ";
      }
    
    // Print AI difficult
    ss << aiDifficulty;
      
    // End stringstream
    //ss << endl;

  // Return string
  return (ss.str());
}

// Takes player string from over network and re-encodes it
void PlayerClass::stringToPlayer(string wrappedString)
{
  istringstream iss;
  iss.str(wrappedString);
  
  int i;  // Loop iter
  int x;  // x loc to be put in BoardLocationClass
  int y;  // y loc to be put in BoardLocationClass
  int x2;
  int y2;
  int lastActionInt;  // last i
  int numCardsInHand;
  int cardInHand;
  int numLocationsVisited;
  int detectCardInt;
  int detectSuspInt;
  int aiDifficultyInt;
  
  // First set player X and Y locations
  iss >> x >> y;
  currentLocation = BoardLocationClass(x,y);
  cout << currentLocation.getXCoord() << " " << currentLocation.getYCoord() << endl;
  
  iss >> hostFlag;
  cout << "hostflag is " << hostFlag << endl;
  iss >> aiFlag;
  cout << "aiFlag is " << aiFlag << endl;
  iss >> movedThisTurnFlag;
  cout << "movedThisTurnFlag is " << movedThisTurnFlag << endl;
  iss >> lastActionInt;
  cout << "lastActionInt is " << lastActionInt << endl;
  lastAction = ActionEnum(lastActionInt);
  
  // Cards in hand
  iss >> numCardsInHand;
  cout << "numCardsInHand is " << numCardsInHand << endl;
  for (i = 0; i < numCardsInHand; i++)
    {
    iss >> cardInHand;
    //cout << CardEnum(cardInHand) << endl;
    hand.insert(CardEnum(cardInHand));
    }
    
      set<CardEnum>::iterator cardIter;
      for (cardIter = hand.begin();
      cardIter != hand.end();
      cardIter++)
      {
      cout << *cardIter << " ";
      }

  iss >> dieRollThisTurn;
  cout << "dieRollThisTurn is " << dieRollThisTurn << endl;
  iss >> movesLeftThisTurn;
  cout << "movesLeftThisTurn is " << movesLeftThisTurn << endl;
  
  // Number of locations visited
  iss >> numLocationsVisited;
  cout << "numLocationsVisited is " << numLocationsVisited << endl;
//  for (i = 0; i < numLocationsVisited; i++)
//  {
    iss >> x2 >> y2;
//    locationsThisTurn.insert(BoardLocationClass(x2,y2));
//  }
  
  // Unwrap detective cards
  for (i = 0; i < NUMBER_OF_CARDS; i++)
  {
    iss >> detectCardInt;
    iss >> detectSuspInt;
    detectiveNotes[i] = make_pair(CardEnum(detectCardInt), SuspectEnum(detectSuspInt));
  }
  
  // Get ai Difficulty - NOT SET
  iss >> aiDifficultyInt;
  cout << "aiDifficulty is " << aiDifficultyInt << endl;
  aiDifficulty = DifficultyEnum(aiDifficultyInt);

  return;
}

//Moves the player over one tile; throws an exception if the move is
//outside the bounds of the board.
void PlayerClass::move(const QImage &currentBoard, const DirectionEnum
    &direction)
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
        throw(ExceptionClass("You have already visited that room this turn.  "
            "Please select another move and try again."));
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
  detectiveNotes[card].second = suspect;
}

set<BoardLocationClass> PlayerClass::getValidExitDoors(const QImage
    &currentBoard)
{
  //Variable Declarations
  set<BoardLocationClass> doors = getDoorsForRoom(currentLocation.getRoom());
  set<BoardLocationClass>::iterator doorIter = doors.begin();
  set<BoardLocationClass> validDoors;
  DirectionEnum direction;
  bool validDoorFlag;

  while(doorIter != doors.end())
  {
    if(doorIter->getTileType(currentBoard) == UNOCCUPIED_TILE)
    {
      validDoors.insert(*doorIter);
    }
    doorIter++;
  }

  return validDoors;
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
    if(currentLocation.getTileInDir(i).checkBoardBounds() == true)
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
  }

  return validDirections;
}


set<ActionEnum> PlayerClass::getValidPrerollMoves()
{
  //Variable Declarations
  set<ActionEnum> validMoves;

  validMoves.insert(ROLL);
  validMoves.insert(ACCUSE);

  if(lastAction == MOVE)
  {
    validMoves.insert(SUGGEST);
  }

  if(currentLocation.checkCornerRoom() == true)
  {
    validMoves.insert(USE_SECRET_PASSAGE);
  }

  return validMoves;
}

ActionEnum PlayerClass::handleAfterRollAi()
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

set<CardEnum> PlayerClass::getSuggestionMatches(SuggestionClass suggestion)
{
  //Variable Declarations
  set<CardEnum> cardMatches;
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

  return cardMatches;
}






//AI CODE STARTS HERE
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

BoardLocationClass PlayerClass::getAiTargetDoor()
{
  multimap<int, BoardLocationClass> targetDoorList =
      currentLocation.getTargetDoors();
  multimap<int, BoardLocationClass>::iterator doorIter = targetDoorList.begin();
  bool gotTargetFlag = false;
  int randomNumber = targetDoorList.size();
  BoardLocationClass target;

  switch(aiDifficulty)
  {
    case VERY_EASY:
      do
      {
        randomNumber = rand() % AI_EASY_TARGET_RAND_MAX;
        if(randomNumber == AI_EASY_TARGET_RAND_MAX - 1)
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
//Insert AI code - set target based on targetDoorList.
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//    case EASY:
//
//      break;
//
//    case MEDIUM:
//
//      break;
//
//    case HARD:
//
//      break;
//
//    case EXPERT:
//
//      break;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
  }
  return target;
}

BoardLocationClass PlayerClass::getAiExitDoor(QImage &currentBoard)
{
  //Variable Declarations
  set<BoardLocationClass> doors = getValidExitDoors(currentBoard);
  set<BoardLocationClass>::iterator doorIter = doors.begin();
  BoardLocationClass doorLocation;
  int doorPosition;

  if(doors.empty() == true)
  {
    throw(ExceptionClass("All doorways blocked"));
  }

  switch(aiDifficulty)
  {
    case VERY_EASY:
      doorPosition = rand() % doors.size();
      for(int i = 0; i < doorPosition; i++)
      {
        doorIter++;
      }
      doorLocation = *doorIter;
      break;

//Insert AI code - set doorLocation based on doors.
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//    case EASY:
//
//      break;
//
//    case MEDIUM:
//
//      break;
//
//    case HARD:
//
//      break;
//
//    case EXPERT:
//
//      break;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
  }

  return doorLocation;
}

DirectionEnum PlayerClass::getAiMove(QImage &currentBoard, BoardLocationClass
    &target)
{
  //Variable Declarations
  OrientationEnum moveOrientation;
  bool validMoveFlag = false;
  queue<DirectionEnum> moveDirection;
  set<DirectionEnum> validDirections = getValidMoveDirections(currentBoard);
  DirectionEnum direction;

  if(validDirections.empty() == true)
  {
    throw(ExceptionClass("No valid move directions"));
  }

  switch(aiDifficulty)
  {
    case VERY_EASY:
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

      direction = moveDirection.front();
      break;
//Insert AI code - set direction based on validDirections.
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//    case EASY:
//
//      break;
//
//    case MEDIUM:
//
//      break;
//
//    case HARD:
//
//      break;
//
//    case EXPERT:
//
//      break;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
  }

  return direction;
}

CardEnum PlayerClass::handleSuggestionAi(SuggestionClass suggestion)
{
  //Variable Declarations
  set<CardEnum> cardMatches = getSuggestionMatches(suggestion);
  set<CardEnum>::iterator cardMatchesIter;
  int randomIndex;
  CardEnum cardToReveal;


  switch(aiDifficulty)
  {
    case VERY_EASY:
      randomIndex = rand() % cardMatches.size();
      cardMatchesIter = cardMatches.begin();
      for(int i = 0; i < randomIndex; i++)
      {
        cardMatchesIter++;
      }
      cardToReveal = *cardMatchesIter;
      break;
//Insert AI code - set cardToReveal based on cardMatches.
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//    case EASY:
//
//      break;
//
//    case MEDIUM:
//
//      break;
//
//    case HARD:
//
//      break;
//
//    case EXPERT:
//
//      break;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
  }
  return cardToReveal;
}

ActionEnum PlayerClass::handlePrerollAi(const QImage &currentBoard)
{
  //Variable Declarations
  set<ActionEnum> validMoves = getValidPrerollMoves();
  set<ActionEnum>::iterator moveIter = validMoves.begin();
  int randomNumber;
  ActionEnum aiAction;

  switch(aiDifficulty)
  {
    case VERY_EASY:
      randomNumber = rand() % validMoves.size();
      for(int i = 0; i < randomNumber; i++)
      {
        moveIter++;
      }
      aiAction = *moveIter;
      break;

//Insert AI code - set aiAction based on the moves in validMoves.
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//    case EASY:
//
//      break;
//
//    case MEDIUM:
//
//      break;
//
//    case HARD:
//
//      break;
//
//    case EXPERT:
//
//      break;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
  }

  return aiAction;
}

SuggestionClass PlayerClass::makeAiSuggestion() const
{
  SuspectEnum suspect;
  WeaponEnum weapon;
  RoomEnum room;

  switch(aiDifficulty)
  {
    case VERY_EASY:
      suspect = SuspectEnum(rand() % NUMBER_OF_SUSPECTS);
      weapon = WeaponEnum(rand() % NUMBER_OF_WEAPONS);
      break;
//Insert AI code - set suspect and weapon.
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//    case EASY:
//
//      break;
//
//    case MEDIUM:
//
//      break;
//
//    case HARD:
//
//      break;
//
//    case EXPERT:
//
//      break;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
  }

  room = currentLocation.getRoom();
  return SuggestionClass(suspect, weapon, room);
}

SuggestionClass PlayerClass::makeAiAccusation() const
{
  bool readyToAccuseFlag = false;
  SuggestionClass accusation;
  SuspectEnum suspect;
  WeaponEnum weapon;
  RoomEnum room;
  int missingEntries = 0;
  int i = 0;

  switch(aiDifficulty)
  {
    case VERY_EASY:
      while(i < NUMBER_OF_CARDS && missingEntries <= NUMBER_OF_CARD_TYPES)
      {
        if(detectiveNotes[i].second == UNKNOWN)
        {
          switch(getCardType(detectiveNotes[i].first))
          {
            case SUSPECT_CARD:
              suspect = cardToSuspect(detectiveNotes[i].first);
              break;
            case WEAPON_CARD:
              weapon = cardToWeapon(detectiveNotes[i].first);
              break;
            case ROOM_CARD:
              room = cardToRoom(detectiveNotes[i].first);
              break;
          }
          missingEntries++;
        }
        i++;
      }

      if(missingEntries == NUMBER_OF_CARD_TYPES)
      {
        readyToAccuseFlag = true;
        accusation = SuggestionClass(suspect, weapon, room);
      }
      break;
//Insert AI code - set accusation or set readyToAccuseFlag to true.
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//    case EASY:
//
//      break;
//
//    case MEDIUM:
//
//      break;
//
//    case HARD:
//
//      break;
//
//    case EXPERT:
//
//      break;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
  }
  if(readyToAccuseFlag == false)
  {
    throw(ExceptionClass("Ai not ready to accuse"));
  }

  return accusation;
}
