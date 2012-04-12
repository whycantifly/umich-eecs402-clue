#include <list>
#include <queue>
#include <set>

#include "PlayerClass.h"
#include "enums.h"
#include "BoardLocationClass.h"
#include "ExceptionClass.h"
#include "getCard.h"
#include "getRoom.h"
#include "getSuspect.h"
#include "getWeapon.h"
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
  correctAiSuggestionFlag = false;
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
  map<CardEnum, set<SuspectEnum> >::iterator cardIter;
  
  // Get number of locations this turn and set up iterator
  int numLocationsVisited = (int) locationsThisTurn.size();
  set<BoardLocationClass>::iterator numLocsIter;
  
  // Open string stream
  stringstream ss;
  
  // Print boring single value stats
  ss << currentLocation.getXCoord() << " " << currentLocation.getYCoord() 
    << " " << hostFlag << " " << aiFlag << " " << gameOverFlag << " " <<
    movedThisTurnFlag << " " << lastAction << " " 
    
    // Print number of cards in hand
    << numCardsInHand << " " ;
    
    // Print cards in this players hand
    for (cardIter = hand.begin();
      cardIter != hand.end();
      cardIter++)
      {
      ss << cardIter->first << " ";
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
    ss << aiDifficulty << " ";
      
    // End stringstream
    ss << endl;

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
  set<SuspectEnum> emptySuspectList;
  
  hand.clear();
  
  // First set player X and Y locations
  iss >> x >> y;
  currentLocation = BoardLocationClass(x,y);
  cout << currentLocation.getXCoord() << " " << currentLocation.getYCoord() << endl;
  
  iss >> hostFlag;
  cout << "hostflag is " << hostFlag << endl;
  iss >> aiFlag;
  cout << "aiFlag is " << aiFlag << endl;
  iss >> gameOverFlag;
  cout << "gameOverFlag is " << gameOverFlag << endl;
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
    hand.insert(pair<CardEnum, set<SuspectEnum> >(CardEnum(cardInHand),
        emptySuspectList));
    }
    
      map<CardEnum, set<SuspectEnum> >::iterator cardIter;
      for (cardIter = hand.begin();
      cardIter != hand.end();
      cardIter++)
      {
      cout << cardIter->first << " ";
      }
      
      cout << endl;

  iss >> dieRollThisTurn;
  cout << "dieRollThisTurn is " << dieRollThisTurn << endl;
  iss >> movesLeftThisTurn;
  cout << "movesLeftThisTurn is " << movesLeftThisTurn << endl;
  
  // Number of locations visited
  iss >> numLocationsVisited;
  cout << "numLocationsVisited is " << numLocationsVisited << endl;
  for (i = 0; i < numLocationsVisited; i++)
  {
    iss >> x2 >> y2;
    locationsThisTurn.insert(BoardLocationClass(x2,y2));
  }
  
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
  //Variable Declarations
  set<SuspectEnum> emptySuspectList;

  hand.insert(pair<CardEnum, set<SuspectEnum> >(cardToAdd, emptySuspectList));
}

void PlayerClass::addToDetectiveNotes(CardEnum card, SuspectEnum suspect)
{
  detectiveNotes[card].second = suspect;
}

multimap<int, BoardLocationClass> PlayerClass::getTargetDoors() const
{
  //Variable Declarations
  multimap<int, BoardLocationClass> targetDoors;
  set<BoardLocationClass> invalidDoors;

  if(currentLocation.getTileType(CLUE_BOARD_IMAGE) == ROOM_TILE)
  {
    invalidDoors = getDoorsForRoom(currentLocation.getRoom());
  }

  for(int i = 0; i < TOTAL_NUMBER_OF_DOORS; i++)
  {
    if(invalidDoors.find(DOOR_LOCATIONS[i]) == invalidDoors.end())
    {
      targetDoors.insert(pair<int, BoardLocationClass>(currentLocation.
          getDistanceTo(DOOR_LOCATIONS[i]), DOOR_LOCATIONS[i]));
    }
  }

  return targetDoors;
}

set<BoardLocationClass> PlayerClass::getValidExitDoors(const QImage
    &currentBoard)
{
  //Variable Declarations
  set<BoardLocationClass> doors = getDoorsForRoom(currentLocation.getRoom());
  set<BoardLocationClass>::iterator doorIter = doors.begin();
  set<BoardLocationClass> validDoors;

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
  try
  {
    makeAiAccusation();
    validMoves.insert(ACCUSE);
  }
  catch(ExceptionClass cannotAccuse)
  {
    //Do nothing
  }

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

map<CardEnum, set<SuspectEnum> > PlayerClass::getSuggestionMatches(
    SuggestionClass &suggestion)
{
  //Variable Declarations
  map<CardEnum, set<SuspectEnum> > cardMatches;
  pair<CardEnum, set<SuspectEnum> > cardToAdd;

  if(hand.find(getCard(suggestion.getSuspect())) != hand.end())
  {
    cardToAdd = *hand.find(getCard(suggestion.getSuspect()));
    cardMatches.insert(cardToAdd);
  }
  if(hand.find(getCard(suggestion.getWeapon())) != hand.end())
  {
    cardToAdd = *hand.find(getCard(suggestion.getWeapon()));
    cardMatches.insert(cardToAdd);
  }
  if(hand.find(getCard(suggestion.getRoom())) != hand.end())
  {
    cardToAdd = *hand.find(getCard(suggestion.getRoom()));
    cardMatches.insert(cardToAdd);
  }

  return cardMatches;
}






//AI CODE STARTS HERE
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

BoardLocationClass PlayerClass::getAiTargetDoor()
{
  multimap<int, BoardLocationClass> targetDoorList = getTargetDoors();
  multimap<int, BoardLocationClass>::iterator doorIter = targetDoorList.begin();
  multimap<int, BoardLocationClass> unknownDoorList;
  set<RoomEnum> unknownRooms;
  bool gotTargetFlag = false;
  int randomNumber = targetDoorList.size();
  BoardLocationClass target;

  if(aiDifficulty != VERY_EASY)
  {
    //Look through detective notes for all unknown rooms
    for(int i = 0; i < NUMBER_OF_ROOMS; i++)
    {
      if (detectiveNotes[getCard(RoomEnum(i))].second == UNKNOWN)
      {
        unknownRooms.insert(RoomEnum(i));
      }
    }
    //At least one unknown room
    if(unknownRooms.empty() == false)
    {
      //Get rid of all doors to known rooms from targetDoorList
      for(doorIter = targetDoorList.begin(); doorIter != targetDoorList.end();
          doorIter++)
      {
        //Door is not a door to an unknown room
        if(unknownRooms.find(doorIter->second.getRoomDoor()) !=
            unknownRooms.end())
        {
          unknownDoorList.insert(*doorIter);
        }
      }
      if(unknownDoorList.empty() == false)
      {
        targetDoorList = unknownDoorList;
      }
    }
  }

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
    case EASY:
      randomNumber = rand() % targetDoorList.size();
      doorIter = targetDoorList.begin();
      for(int i = 0; i < randomNumber; i++)
      {
        doorIter++;
      }
      target = doorIter->second;
    default:
      target = targetDoorList.begin()->second;
      break;
  }
  return target;
}

BoardLocationClass PlayerClass::getAiExitDoor(QImage &currentBoard,
    BoardLocationClass &target)
{
  //Variable Declarations
  set<BoardLocationClass> doors = getValidExitDoors(currentBoard);
  set<BoardLocationClass>::iterator doorIter = doors.begin();
  set<BoardLocationClass>::const_iterator closestDoorIter = doors.end();
  BoardLocationClass doorLocation;
  int doorPosition;

  if(doors.empty() == true)
  {
    throw(ExceptionClass("All doorways blocked"));
  }

  switch(aiDifficulty)
  {
    //Use a random exit door
    case VERY_EASY:
      doorPosition = rand() % doors.size();
      for(int i = 0; i < doorPosition; i++)
      {
        doorIter++;
      }
      doorLocation = *doorIter;
      break;
    //Use the exit door that's closest to the target
    default:
      for(doorIter = doors.begin(); doorIter != doors.end(); doorIter++)
      {
        if(closestDoorIter == doors.end() || doorIter->getDistanceTo(target) <
            closestDoorIter->getDistanceTo(target))
        {
          closestDoorIter = doorIter;
        }
      }
      doorLocation = *closestDoorIter;
      break;
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
    case EASY:                  //Comment me out
    case MEDIUM:                //Comment me out
    case HARD:                  //Comment me out
    case EXPERT:                //Comment me out
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
  map<CardEnum, set<SuspectEnum> > cardMatches =
      getSuggestionMatches(suggestion);
  map<CardEnum, set<SuspectEnum> >::iterator cardMatchesIter;
  map<CardEnum, set<SuspectEnum> > alreadyShownCards;
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
      cardToReveal = cardMatchesIter->first;
    break;
    default:
      //Look at current player's detective notes.  If a suggestion has already
      //been shown, then show that card.  Otherwise, show a random card.
      cardMatchesIter = cardMatches.begin();

      if(cardMatches.size() > 1)
      {
        while(cardMatchesIter != cardMatches.end())
        {
          if(cardMatchesIter->second.empty() == true)
          {
            alreadyShownCards.insert(*cardMatchesIter);
          }
          cardMatchesIter++;
        }

        if(alreadyShownCards.empty() == false)
        {
          cardMatchesIter = alreadyShownCards.begin();
          randomIndex = rand() % alreadyShownCards.size();
        }
        else
        {
          cardMatchesIter = cardMatches.begin();
          randomIndex = rand() % cardMatches.size();
        }

        for(int i = 0; i < randomIndex; i++)
        {
          cardMatchesIter++;
        }
      }
      cardToReveal = cardMatchesIter->first;
      break;
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
    case EASY:                //Comment me out
    case MEDIUM:              //Comment me out
    case HARD:                //Comment me out
    case EXPERT:              //Comment me out
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
  set<SuspectEnum> unknownSuspects;
  set<WeaponEnum> unknownWeapons;
  set<RoomEnum> unknownRooms;

  switch(aiDifficulty)
  {
    case VERY_EASY:
      suspect = SuspectEnum(rand() % NUMBER_OF_SUSPECTS);
      weapon = WeaponEnum(rand() % NUMBER_OF_WEAPONS);
      break;
    default:
      //Get all unknown suspects
      for(int i = 0; i < NUMBER_OF_SUSPECTS; i++)
      {
        if(detectiveNotes[getCard(SuspectEnum(i))].second == UNKNOWN)
        {
          unknownSuspects.insert(SuspectEnum(i));
        }
      }
      //Get all unknown weapons
      for(int i = 0; i < NUMBER_OF_WEAPONS; i++)
      {
        if(detectiveNotes[getCard(WeaponEnum(i))].second == UNKNOWN)
        {
          unknownWeapons.insert(WeaponEnum(i));
        }
      }

      if(unknownSuspects.empty() == false)
      {
        do
        {
          suspect = SuspectEnum(rand() % NUMBER_OF_SUSPECTS);
        }
        while(unknownSuspects.find(suspect) == unknownSuspects.end());
      }
      else
      {
        suspect = SuspectEnum(rand() % NUMBER_OF_SUSPECTS);
      }

      if(unknownWeapons.empty() == false)
      {
        do
        {
          weapon = WeaponEnum(rand() % NUMBER_OF_WEAPONS);
        }
        while(unknownWeapons.find(weapon) == unknownWeapons.end());
      }
      else
      {
        weapon = WeaponEnum(rand() % NUMBER_OF_WEAPONS);
      }
      break;
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

  if(correctAiSuggestionFlag == true)
  {
    accusation = aiAccusation;
  }
  else
  {
    while(i < NUMBER_OF_CARDS && missingEntries <= NUMBER_OF_CARD_TYPES)
    {
      if(detectiveNotes[i].second == UNKNOWN)
      {
        switch(getCardType(detectiveNotes[i].first))
        {
          case SUSPECT_CARD:
            suspect = getSuspect(detectiveNotes[i].first);
            break;
          case WEAPON_CARD:
            weapon = getWeapon(detectiveNotes[i].first);
            break;
          case ROOM_CARD:
            room = getRoom(detectiveNotes[i].first);
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

    if(readyToAccuseFlag == false)
    {
      throw(ExceptionClass("Ai not ready to accuse"));
    }
  }

  return accusation;
}
