#include <cmath>
#include <queue>

#include "BoardLocationClass.h"
#include "constants.h"
#include "enums.h"
#include "ExceptionClass.h"

using namespace std;

const QRgb BoardLocationClass::getTileColor(const QImage &currentBoard) const
{
  return currentBoard.pixel(getMiddlePixel().x(), getMiddlePixel().y());
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

  i = 0;
  while(i < NUMBER_OF_SUSPECTS + 1 && foundTile == false)
  {
    if(tileColor == EMPTY_TILE_RGB[i])
    {
      foundTile = true;
      tileType = UNOCCUPIED_TILE;
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
    while(i < NUMBER_OF_ROOMS)
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
  if(xCoord < 0 || xCoord > BOARD_WIDTH || yCoord < 0 || yCoord > BOARD_HEIGHT)
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
        blockedFlag == false;
      }

      i++;
    }
  }
  else
  {
    while(moveDir <= RIGHT && blockedFlag == true)
    {
      newLocation = *this;
      newLocation.move(currentBoard, moveDir);

      if(newLocation.getTileType(CLUE_BOARD_IMAGE) == ROOM_TILE ||
          newLocation.getTileType(currentBoard) == UNOCCUPIED_TILE)
      {
        blockedFlag = false;
      }

      moveDir = DirectionEnum(moveDir + 1);
    }
  }

  return blockedFlag;
}

void BoardLocationClass::move(const QImage &currentBoard,
    const DirectionEnum &direction)
{
  BoardLocationClass newLocation = *this;
  bool enteredRoomFlag = false;
  int i;

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
  if(checkBoardBounds() == false)
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

  if(newLocation.getTileType(currentBoard) == ROOM_TILE)
  {
    i = 0;
    while(i < TOTAL_NUMBER_OF_DOORS && enteredRoomFlag == false)
    {
      if(*this == DOOR_LOCATIONS[i] && direction == DOOR_DIRECTIONS[i])
      {
        enteredRoomFlag == true;
        newLocation = newLocation.getEmptyRoomTile(currentBoard);
      }
      i++;
    }

    if(enteredRoomFlag == false)
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

BoardLocationClass BoardLocationClass::getClosestDoor()
{
  BoardLocationClass closestDoor = DOOR_LOCATIONS[0];

  for(int i = 1; i < TOTAL_NUMBER_OF_DOORS; i++)
  {
    if(abs(DOOR_LOCATIONS[i].xCoord - xCoord) +
        abs(DOOR_LOCATIONS[i].yCoord - yCoord) <
        abs(closestDoor.xCoord - xCoord) + abs(closestDoor.yCoord - yCoord))
    {
      closestDoor = DOOR_LOCATIONS[i];
    }
  }
}

queue<DirectionEnum> BoardLocationClass::getMovesTo(const QImage &currentBoard,
    int movesLeft, BoardLocationClass origin) const
{
  queue<DirectionEnum> moveList;

  for(int i = 0; i < movesLeft; i++)
  {
    if(origin.xCoord < xCoord)
    {

    }
    else if(origin.xCoord > xCoord)
    {

    }
    else
    {
      if(origin.yCoord < yCoord)
      {

      }
      else if(origin.yCoord > yCoord)
      {

      }
      else
      {

      }
    }
  }
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
