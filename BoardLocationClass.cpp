#include <cmath>
#include <queue>
#include <set>

#include "BoardLocationClass.h"
#include "constants.h"
#include "enums.h"
#include "ExceptionClass.h"

using namespace std;

const QRgb BoardLocationClass::getTileColor(const QImage &currentBoard) const
{
  return currentBoard.pixel(getMiddlePixel());
}

const TileTypeEnum BoardLocationClass::getTileType(const QImage &currentBoard)
    const
{
  //Variable Declarations
  bool foundTile = false;
  QRgb tileColor;
  TileTypeEnum tileType;
  int i;

  tileColor = getTileColor(currentBoard);

  if(tileColor == EMPTY_TILE_RGB)
  {
    foundTile = true;
    tileType = UNOCCUPIED_TILE;
  }

  i = 0;
  while(i < NUMBER_OF_SUSPECTS && foundTile == false)
  {
    if(tileColor == VISITED_TILE_RGB[i])
    {
      foundTile = true;
      tileType = VISITED_TILE;
    }
    i++;
  }

  i = 0;
  while(i < NUMBER_OF_ROOMS && foundTile == false)
  {
    if(tileColor == ROOM_TILE_RGB[i])
    {
      foundTile = true;
      tileType = ROOM_TILE;
    }
    i++;
  }

  i = 0;
  while(i < NUMBER_OF_SUSPECTS && foundTile == false)
  {
    if(tileColor == CHAR_PIECE_RGB[i])
    {
      foundTile = true;
      tileType = OCCUPIED_TILE;
    }
    i++;
  }

  if(foundTile == false)
  {
    tileType = OUT_OF_BOUNDS_TILE;
  }

  return tileType;
}

const QPoint BoardLocationClass::getTopLeftPixel() const
{
  return QPoint(xCoord * (TILE_WIDTH - 1),
      yCoord * (TILE_HEIGHT - 1));
}

const QPoint BoardLocationClass::getMiddlePixel() const
{
  return QPoint(xCoord * (TILE_WIDTH - 1) + TILE_WIDTH / 2,
      yCoord * (TILE_HEIGHT - 1) + TILE_HEIGHT / 2);
}

bool BoardLocationClass::checkCornerRoom() const
{
  if(getRoom() == LOUNGE || getRoom() == KITCHEN || getRoom() == CONSERVATORY ||
      getRoom() == STUDY)
  {
    return true;
  }
  else
  {
    return false;
  }
}


const RoomEnum BoardLocationClass::getRoom() const
{
  //Variable Declarations
  QRgb tileColor;
  RoomEnum room;
  bool foundRoom = false;
  int i;

  if(getTileType(CLUE_BOARD_IMAGE) != ROOM_TILE)
  {
    throw(ExceptionClass(QString("That tile is not a room tile.  Please try "
        "again.")));
  }
  else
  {
    tileColor = getTileColor(CLUE_BOARD_IMAGE);

    i = 0;
    while(i < NUMBER_OF_ROOMS && foundRoom == false)
    {
      if(tileColor == ROOM_TILE_RGB[i])
      {
        foundRoom = true;
        room = RoomEnum(i);
      }
      i++;
    }
  }
  return room;
}

bool BoardLocationClass::checkBoardBounds() const
{
  if(xCoord < 0 || xCoord >= BOARD_WIDTH || yCoord < 0 || yCoord >= BOARD_HEIGHT)
  {
    return false;
  }
  else
  {
    return true;
  }
}

bool BoardLocationClass::checkPlayerBlocked(const QImage &currentBoard) const
{
  bool blockedFlag = true;
  BoardLocationClass newLocation;
  DirectionEnum moveDir = UP;
  int doorStart = 0;
  int i;

  if(getTileType(CLUE_BOARD_IMAGE) == ROOM_TILE)
  {
    for(i = 0; i < int(getRoom()); i++)
    {
      doorStart += NUMBER_OF_DOORS[i];
    }

    i = 0;
    while(i < NUMBER_OF_DOORS[int(getRoom())] && blockedFlag == true)
    {
      if(DOOR_LOCATIONS[doorStart + i].getTileType(currentBoard) !=
          OCCUPIED_TILE)
      {
        blockedFlag = false;
      }

      i++;
    }
  }
  else
  {
    while(moveDir <= RIGHT && blockedFlag == true)
    {
      newLocation = *this;
      try
      {
        newLocation.move(currentBoard, moveDir);
        blockedFlag = false;
      }
      catch(ExceptionClass invalidDirection)
      {
      }
      moveDir = DirectionEnum(moveDir + 1);
    }
  }

  return blockedFlag;
}

void BoardLocationClass::move(const QImage &currentBoard,
    const DirectionEnum &direction)
{
  BoardLocationClass newLocation = getTileInDir(direction);
  bool doorFlag = false;
  int startingDoorIndex = 0;
  int i;

  if(newLocation.checkBoardBounds() == false)
  {
    throw(ExceptionClass("Invalid Move", "That tile is not within the bounds "
        "of the board.  Please try another move."));
  }

  if(newLocation.getTileType(currentBoard) == OUT_OF_BOUNDS_TILE)
  {
    throw(ExceptionClass("That tile is not within the bounds of the "
        "board.  Please try another move."));
  }

  if(newLocation.getTileType(currentBoard) == OCCUPIED_TILE)
  {
    throw(ExceptionClass("That tile is occupied by another player.  "
        "Please try another move."));
  }

  if(newLocation.getTileType(currentBoard) == VISITED_TILE)
  {
    throw(ExceptionClass("You have already visited that tile this turn."));
  }

  if(newLocation.getTileType(currentBoard) == ROOM_TILE)
  {
    i = 0;
    while(i < TOTAL_NUMBER_OF_DOORS && doorFlag == false)
    {
      if(*this == DOOR_LOCATIONS[i] && direction == DOOR_DIRECTIONS[i])
      {
        for(int j = 0; j < int(newLocation.getRoom()); j++)
        {
          startingDoorIndex += NUMBER_OF_DOORS[j];
        }

        for(int j = 0; j < NUMBER_OF_DOORS[newLocation.getRoom()]; j++)
        {
          if(DOOR_LOCATIONS[startingDoorIndex + j].getTileType(currentBoard) ==
              VISITED_TILE)
          {
            throw(ExceptionClass("You have already visited this room this "
                "turn.  Please try another move."));
          }
        }
        doorFlag = true;
        newLocation = newLocation.getEmptyRoomTile(currentBoard);
      }
      i++;
    }

    if(doorFlag == false)
    {
      throw(ExceptionClass("That tile is on the other side of a wall.  You "
          "must use a door to enter a room.  Please try another move."));
    }
  }
  *this = newLocation;
}

RoomEnum BoardLocationClass::getRoomDoor() const
{
  int i;
  int j;
  bool isDoorTile = false;
  int runningNumberOfDoors = 0;
  RoomEnum room;

  i = 0;
  while(i < TOTAL_NUMBER_OF_DOORS && isDoorTile == false)
  {
    if(BoardLocationClass(xCoord, yCoord) == DOOR_LOCATIONS[i])
    {
      j = 0;
      for(j = 0; runningNumberOfDoors <= i; j++)
      {
        runningNumberOfDoors += NUMBER_OF_DOORS[j];
      }
      room = RoomEnum(j - 1);
      isDoorTile = true;
    }
    i++;

  }
  if(isDoorTile == false)
  {
    throw(ExceptionClass("That tile is on the other side of a wall.  You "
        "must use a door to enter a room.  Please try another move."));
  }

  return room;
}

int BoardLocationClass::getClosestDoorIndex() const
{
  BoardLocationClass closestDoor = DOOR_LOCATIONS[0];
  int closestDoorIndex = 0;

  for(int i = 1; i < TOTAL_NUMBER_OF_DOORS; i++)
  {
    if(abs(DOOR_LOCATIONS[i].xCoord - xCoord) +
        abs(DOOR_LOCATIONS[i].yCoord - yCoord) <
        abs(closestDoor.xCoord - xCoord) + abs(closestDoor.yCoord - yCoord))
    {
      closestDoor = DOOR_LOCATIONS[i];
      closestDoorIndex = i;
    }
  }

  return closestDoorIndex;
}

queue<DirectionEnum> BoardLocationClass::getMovesToDoor(const QImage &currentBoard,
    int movesLeft, int doorIndex) const
{
  queue<DirectionEnum> moveList;
  BoardLocationClass origin = *this;
  BoardLocationClass target = DOOR_LOCATIONS[doorIndex];
  OrientationEnum moveOrientation;
  deque<DirectionEnum> moveDirection;
  bool movedFromOriginFlag = false;
  bool visitedRoomFlag = false;
  int startingDoorIndex = 0;
  int i;
  RoomEnum roomVisited = HALL;
  set<BoardLocationClass> alreadyVisited;
  set<BoardLocationClass>::iterator alreadyVisitedIter;
  set<DirectionEnum> opposingDirections;
  set<DirectionEnum>::iterator opposingIter;
  DirectionEnum frontDirection;
  DirectionEnum backDirection;

  alreadyVisited.insert(*this);

  //If move is starting on a door tile, add all doors to that room to the
  //already visited list.
  while(visitedRoomFlag == false && roomVisited <= STUDY)
  {
    i = 0;
    while(i < NUMBER_OF_DOORS[roomVisited] && visitedRoomFlag == false)
    {
      if(DOOR_LOCATIONS[startingDoorIndex + i] == *this)
      {
        visitedRoomFlag = true;
      }
      i++;
    }
    if(visitedRoomFlag == false)
    {
      startingDoorIndex += NUMBER_OF_DOORS[roomVisited];
      roomVisited = RoomEnum(int(roomVisited) + 1);
    }
    else
    {
      for(i = 0; i < NUMBER_OF_DOORS[roomVisited]; i++)
      {
        alreadyVisited.insert(DOOR_LOCATIONS[startingDoorIndex + i]);
      }
    }
  }

  //Decide whether to make a horizontal or vertical move first
  if(origin.xCoord == target.xCoord)
  {
    moveOrientation = VERTICAL;
  }
  else if(origin.yCoord == target.yCoord)
  {
    moveOrientation = HORIZONTAL;
  }
  else
  {
    moveOrientation = OrientationEnum(rand() % 2);
  }


  while(movesLeft > 0)
  {
    //Reached the target door
    if(origin == target)
    {
      moveList.push(DOOR_DIRECTIONS[doorIndex]);
      movesLeft = 0;
    }
    //Blocked
    else if(checkPlayerBlocked(currentBoard) == true)
    {
      movesLeft = 0;
    }
    //Can move
    else
    {
      while(moveDirection.size() < NUMBER_OF_DIRECTIONS)
      {
        switch(moveOrientation)
        {
          case HORIZONTAL:
            if(origin.yCoord < target.yCoord)
            {
              frontDirection = DOWN;
              backDirection = UP;
            }
            else if(origin.yCoord > target.yCoord)
            {
              frontDirection = UP;
              backDirection = DOWN;
            }
            else
            {
              if(origin.yCoord >= BOARD_HEIGHT / 2)
              {
                frontDirection = UP;
                backDirection = DOWN;
              }
              else
              {
                frontDirection = DOWN;
                backDirection = UP;
              }
            }
            moveOrientation = VERTICAL;
            break;

          case VERTICAL:
            if(origin.xCoord < target.xCoord)
            {
              frontDirection = RIGHT;
              backDirection = LEFT;
            }
            else if(origin.xCoord > target.xCoord)
            {
              frontDirection = LEFT;
              backDirection = RIGHT;
            }
            else
            {
              if(origin.xCoord >= BOARD_WIDTH / 2)
              {
                frontDirection = LEFT;
                backDirection = RIGHT;
              }
              else
              {
                frontDirection = RIGHT;
                backDirection = LEFT;
              }

            }
            moveOrientation = HORIZONTAL;
            break;
        }
        if(opposingDirections.find(frontDirection) == opposingDirections.end())
        {
          moveDirection.push_front(frontDirection);
        }

        if(opposingDirections.find(backDirection) == opposingDirections.end())
        {
          moveDirection.push_back(backDirection);
        }
      }

      opposingIter = opposingDirections.begin();
      while(opposingIter != opposingDirections.end())
      {
        moveDirection.push_back(*opposingIter);
        opposingIter++;
      }

      while(moveDirection.size() > 0 && movedFromOriginFlag == false)
      {
        if((origin.getTileInDir(moveDirection.front()).getTileType(currentBoard)
            == UNOCCUPIED_TILE) && alreadyVisited.find(origin.getTileInDir(
            moveDirection.front())) == alreadyVisited.end())
        {
          if(moveDirection.front() == UP || moveDirection.front() == DOWN)
          {
            moveOrientation = VERTICAL;
            if(moveDirection.front() == UP)
            {
              opposingDirections.insert(DOWN);
            }
            else
            {
              opposingDirections.insert(UP);
            }
          }
          else
          {
            moveOrientation = HORIZONTAL;
            if(moveDirection.front() == LEFT)
            {
              opposingDirections.insert(RIGHT);
            }
            else
            {
              opposingDirections.insert(LEFT);
            }
          }

          origin.move(currentBoard, moveDirection.front());
          alreadyVisited.insert(origin);
          moveList.push(moveDirection.front());
          movesLeft--;
          movedFromOriginFlag = true;
        }

        moveDirection.pop_front();
      }

      if(movedFromOriginFlag == false && moveDirection.size() == 0)
      {
        movesLeft = 0;
      }

      moveDirection.clear();
      movedFromOriginFlag = false;
    }
  }

  return moveList;
}

const BoardLocationClass BoardLocationClass::getEmptyRoomTile(
    const QImage &currentBoard) const
{
  //Variable Declarations
  bool emptyTileFlag = false;
  BoardLocationClass currentLocation;
  int i;
  int j;

  i = 0;
  while(i < ROOM_STORAGE_WIDTH && emptyTileFlag == false)
  {
    j = 0;
    while(j < NUMBER_OF_SUSPECTS / ROOM_STORAGE_WIDTH && emptyTileFlag == false)
    {
      currentLocation =
          BoardLocationClass(ROOM_PIECE_LOCATIONS[int(getRoom())].getXCoord() +
          i, ROOM_PIECE_LOCATIONS[int(getRoom())].getYCoord() + j);
      if(currentLocation.getTileType(currentBoard) == ROOM_TILE)
      {
        emptyTileFlag = true;
      }
      j++;
    }
    i++;
  }

  return currentLocation;
}

BoardLocationClass BoardLocationClass::getTileInDir(DirectionEnum direction)
    const
{
  BoardLocationClass newLocation = *this;

  switch(direction)
  {
    case UP:
      newLocation.yCoord -= 1;
      break;
    case DOWN:
      newLocation.yCoord += 1;
      break;
    case LEFT:
      newLocation.xCoord -= 1;
      break;
    case RIGHT:
      newLocation.xCoord += 1;
      break;
  }

  return newLocation;
}
