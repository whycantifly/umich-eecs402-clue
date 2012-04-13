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
  resetLocationsThisTurn();

  for(int i = 0; i < NUMBER_OF_CARDS; i++)
  {
    detectiveNotes[i] = make_pair(CardEnum(i), UNKNOWN_SUSPECT);
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
    &currentBoard) const
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
    &currentBoard) const
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
    SuggestionClass &suggestion) const
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

BoardLocationClass PlayerClass::getAiTargetDoor() const
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
      if (detectiveNotes[getCard(RoomEnum(i))].second == UNKNOWN_SUSPECT)
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
    BoardLocationClass &target) const
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
    &target) const
{
  //Variable Declarations
  OrientationEnum moveOrientation;          //Orientation suspect prefers to
                                            //move in
  bool validMoveFlag = false;               //True = valid move has been found
  queue<DirectionEnum> moveDirection;       //List of move directions
  set<DirectionEnum> validDirections =      //List of all valid move
      getValidMoveDirections(currentBoard); //directions
  DirectionEnum direction;                  //Move direction

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
      switch(aiDifficulty)
      {
        case VERY_EASY:
          moveOrientation = OrientationEnum(rand() %
              (NUMBER_OF_DIRECTIONS / 2));
          break;
        default:
          if (target.getYCoord() != currentLocation.getYCoord())
          {
            moveOrientation = VERTICAL;
          }
          else
          {
            moveOrientation = HORIZONTAL;
          }
          break;
      }
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
  return direction;
}

CardEnum PlayerClass::handleSuggestionAi(SuggestionClass suggestion, SuspectEnum
    currentSuspect) const
{
  //Variable Declarations
  map<CardEnum, set<SuspectEnum> > cardMatches =
      getSuggestionMatches(suggestion);
  map<CardEnum, set<SuspectEnum> >::iterator cardMatchesIter;
  map<CardEnum, set<SuspectEnum> > alreadyShownCards;
  int playersShown = -1;
  int cardIndex;
  bool cardPickedFlag = false;
  CardEnum cardToReveal;


  switch(aiDifficulty)
  {
    case VERY_EASY:
    case EASY:
      cardIndex = rand() % cardMatches.size();
      cardMatchesIter = cardMatches.begin();
      for(int i = 0; i < cardIndex; i++)
      {
        cardMatchesIter++;
      }
      cardToReveal = cardMatchesIter->first;
    break;
    case MEDIUM:
      //Look at current player's detective notes.  If one or more cards that
      //match the suggestion has already been shown, then show one of those
      //cards.  Otherwise, show a random card.
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

          switch(aiDifficulty)
          {
            case MEDIUM:
              cardIndex = rand() % alreadyShownCards.size();
              break;
            default:
              //Show the first card the player has already seen
              cardIndex = 0;
              while(cardMatchesIter != alreadyShownCards.end() &&
                  cardPickedFlag == false)
              {
                if(cardMatchesIter->second.find(currentSuspect) !=
                    cardMatchesIter->second.end())
                {
                  cardPickedFlag = true;
                }
                else
                {
                  cardIndex++;
                  cardMatchesIter++;
                }
              }

              //Player has not seen any matching cards, so pick the first
              //card that players have seen the most.
              cardMatchesIter = alreadyShownCards.begin();
              if(cardPickedFlag == false)
              {
                for(int i = 0; i < alreadyShownCards.size(); i++)
                {
                  if(cardMatchesIter->second.size() > playersShown)
                  {
                    playersShown = cardMatchesIter->second.size();
                    cardIndex = i;
                  }
                }
              }
              break;
          }
        }
        else
        {
          cardMatchesIter = cardMatches.begin();
          cardIndex = rand() % cardMatches.size();
        }

        for(int i = 0; i < cardIndex; i++)
        {
          cardMatchesIter++;
        }
      }
      cardToReveal = cardMatchesIter->first;
      break;
  }
  return cardToReveal;
}

ActionEnum PlayerClass::handlePrerollAi()
{
  //Variable Declarations
  set<ActionEnum> validMoves = getValidPrerollMoves();
  set<ActionEnum>::iterator moveIter = validMoves.begin();
  set<RoomEnum> unknownRooms;
  int randomNumber;
  ActionEnum aiAction = END_TURN;

  //Get all unknown rooms
  for(int i = 0; i < NUMBER_OF_ROOMS; i++)
  {
    if(detectiveNotes[getCard(RoomEnum(i))].second == UNKNOWN_SUSPECT)
    {
      unknownRooms.insert(RoomEnum(i));
    }
  }

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
    default:
      if(validMoves.find(ACCUSE) != validMoves.end())
      {
        aiAction = ACCUSE;
      }
      else if(validMoves.find(SUGGEST) != validMoves.end() && (aiDifficulty ==
          EASY || unknownRooms.find(currentLocation.getRoom()) !=
          unknownRooms.end()))
      {
        aiAction = SUGGEST;
      }
      else if(validMoves.find(USE_SECRET_PASSAGE) != validMoves.end() &&
          ((currentLocation.getRoom() == LOUNGE &&
          unknownRooms.find(CONSERVATORY) != unknownRooms.end()) ||
          (currentLocation.getRoom() == CONSERVATORY &&
          unknownRooms.find(LOUNGE) != unknownRooms.end()) ||
          (currentLocation.getRoom() == KITCHEN &&
          unknownRooms.find(STUDY) != unknownRooms.end()) ||
          (currentLocation.getRoom() == STUDY &&
          unknownRooms.find(KITCHEN) != unknownRooms.end())))
      {
        aiAction = USE_SECRET_PASSAGE;
      }
      else
      {
        aiAction = ROLL;
      }
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
        if(detectiveNotes[getCard(SuspectEnum(i))].second == UNKNOWN_SUSPECT)
        {
          unknownSuspects.insert(SuspectEnum(i));
        }
      }
      //Get all unknown weapons
      for(int i = 0; i < NUMBER_OF_WEAPONS; i++)
      {
        if(detectiveNotes[getCard(WeaponEnum(i))].second == UNKNOWN_SUSPECT)
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

void PlayerClass::makeAiAccusation()
{
  SuspectEnum suspect = UNKNOWN_SUSPECT;
  WeaponEnum weapon = UNKNOWN_WEAPON;
  RoomEnum room = UNKNOWN_ROOM;
  bool oneUnknownFlag;
  int unknownCards = 0;
  int i;

  switch(aiDifficulty)
  {
    case VERY_EASY:
      aiAccusation = SuggestionClass();
      i = 0;
      while(i < NUMBER_OF_CARDS && unknownCards <= NUMBER_OF_CARD_TYPES)
      {
        if(detectiveNotes[i].second == UNKNOWN_SUSPECT)
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
          unknownCards++;
        }
        i++;
      }

      if(unknownCards == NUMBER_OF_CARD_TYPES)
      {
        aiAccusation = SuggestionClass(suspect, weapon, room);
      }
      break;

    default:
      i = 0;
      oneUnknownFlag = true;
      while(i < NUMBER_OF_SUSPECTS && oneUnknownFlag == true && aiAccusation.
          getSuspect() == UNKNOWN_SUSPECT)
      {
        if(detectiveNotes[getCard(SuspectEnum(i))].second == UNKNOWN_SUSPECT)
        {
          if(suspect == UNKNOWN_SUSPECT)
          {
            suspect = getSuspect(detectiveNotes[getCard(SuspectEnum(i))].first);
          }
          else
          {
            oneUnknownFlag = false;
          }
        }
        i++;
      }

      if(oneUnknownFlag == true && aiAccusation.getSuspect() == UNKNOWN_SUSPECT)
      {
        aiAccusation.setSuspect(suspect);

        i = 0;
        while(i < NUMBER_OF_WEAPONS && oneUnknownFlag == true && aiAccusation.
            getWeapon() == UNKNOWN_WEAPON)
        {
          if(detectiveNotes[getCard(WeaponEnum(i))].second == UNKNOWN_SUSPECT)
          {
            if(weapon == UNKNOWN_WEAPON)
            {
              weapon = getWeapon(detectiveNotes[getCard(WeaponEnum(i))].first);
            }
            else
            {
              oneUnknownFlag = false;
            }
          }
          i++;
        }

        if(oneUnknownFlag == true && aiAccusation.getWeapon() == UNKNOWN_WEAPON)
        {
          aiAccusation.setWeapon(weapon);

          i = 0;
          while(i < NUMBER_OF_ROOMS && oneUnknownFlag == true && aiAccusation.
              getRoom() == UNKNOWN_ROOM)
          {
            if(detectiveNotes[getCard(RoomEnum(i))].second == UNKNOWN_SUSPECT)
            {
              if(room == UNKNOWN_ROOM)
              {
                room = getRoom(detectiveNotes[getCard(RoomEnum(i))].first);
              }
              else
              {
                oneUnknownFlag = false;
              }
            }
            i++;
          }

          if(oneUnknownFlag == true && aiAccusation.getRoom() == UNKNOWN_ROOM)
          {
            aiAccusation.setRoom(room);
          }
        }
      }
      break;
  }

  if(aiAccusation.getSuspect() == UNKNOWN_SUSPECT || aiAccusation.getWeapon()
      == UNKNOWN_WEAPON || aiAccusation.getRoom() == UNKNOWN_ROOM)
  {
    throw(ExceptionClass("Ai not ready to accuse"));
  }
}

void PlayerClass::setAiAccusation(const SuggestionClass &suggestion)
{
  if(hand.find(getCard(suggestion.getSuspect())) == hand.end())
  {
    aiAccusation.setSuspect(suggestion.getSuspect());
  }

  if(hand.find(getCard(suggestion.getWeapon())) == hand.end())
  {
    aiAccusation.setWeapon(suggestion.getWeapon());
  }

  if(hand.find(getCard(suggestion.getRoom())) == hand.end())
  {
    aiAccusation.setRoom(suggestion.getRoom());
  }
}
