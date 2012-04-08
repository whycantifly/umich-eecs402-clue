#include <cmath>
#include <queue>
#include <set>

#include "BoardLocationClass.h"
#include "constants.h"
#include "enums.h"
#include "getDoorsForRoom.h"
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
  RoomEnum room = HALL;
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

void BoardLocationClass::move(const QImage &currentBoard,
    const DirectionEnum &direction)
{
  BoardLocationClass newLocation = getTileInDir(direction);

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
    try
    {
      getDoorIndex();
      newLocation = newLocation.getEmptyRoomTile(currentBoard);
    }
    catch(ExceptionClass notADoor)
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
  RoomEnum room = HALL;

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

multimap<int, BoardLocationClass> BoardLocationClass::getTargetDoors() const
{
  //Variable Declarations
  multimap<int, BoardLocationClass> targetDoors;
  set<BoardLocationClass> invalidDoors;

  if(getTileType(CLUE_BOARD_IMAGE) == ROOM_TILE)
  {
    invalidDoors = getDoorsForRoom(getRoom());
  }

  for(int i = 0; i < TOTAL_NUMBER_OF_DOORS; i++)
  {
    if(invalidDoors.find(DOOR_LOCATIONS[i]) == invalidDoors.end())
    {
      targetDoors.insert(pair<int, BoardLocationClass>(abs(DOOR_LOCATIONS[i].
          getXCoord() - xCoord) + abs(DOOR_LOCATIONS[i].getYCoord() - yCoord),
          DOOR_LOCATIONS[i]));
    }
  }

  return targetDoors;
}

RoomEnum BoardLocationClass::getRoomForDoor(BoardLocationClass doorLocation)
const
{
  int roomIndex = 0;
  bool foundRoomFlag = false;
  set<BoardLocationClass> doors;

  while(roomIndex < NUMBER_OF_ROOMS && foundRoomFlag == false)
  {
    doors = getDoorsForRoom(RoomEnum(roomIndex));

    if(doors.find(doorLocation) != doors.end())
    {
      foundRoomFlag = true;
    }
    else
    {
      roomIndex++;
    }
  }

  if(foundRoomFlag == false)
  {
    throw(ExceptionClass("That tile is not a door tile."));
  }

  return RoomEnum(roomIndex);
}

RoomEnum BoardLocationClass::getRoomForDoor(int doorIndex) const
{
  return(getRoomForDoor(DOOR_LOCATIONS[doorIndex]));
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

int BoardLocationClass::getDoorIndex() const
{
  //Variable Declarations
  int i = 0;
  bool doorFoundFlag = false;

  while(i < TOTAL_NUMBER_OF_DOORS && doorFoundFlag == false)
  {
    if(*this == DOOR_LOCATIONS[i])
    {
      doorFoundFlag = true;
    }
    else
    {
      i++;
    }
  }

  if(doorFoundFlag == false)
  {
    throw(ExceptionClass("This location is not a door."));
  }

  return i;
}
