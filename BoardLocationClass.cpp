#include "BoardLocationClass.h"
#include "constants.h"
#include "enums.h"
#include "ExceptionClass.h"

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

const CardEnum BoardLocationClass::getRoom() const
{
  //Variable Declarations
  QRgb tileColor;
  CardEnum room;
  bool foundRoom = false;
  int i;

  if(getTileType(CLUE_BOARD_IMAGE) != ROOM_TILE)
  {
    throw(ExceptionClass(QString("No room")));
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
        room = CardEnum(NUMBER_OF_SUSPECTS + NUMBER_OF_WEAPONS + i);
      }
      i++;
    }
  }
  return room;
}

void BoardLocationClass::checkBoardBounds()
{
  if(xCoord < 0 || xCoord > BOARD_WIDTH || yCoord < 0 || yCoord > BOARD_HEIGHT)
  {
    throw(ExceptionClass("Invalid Move", "That tile is not within the bounds of "
        "the board.  Please try another move."));
  }
}

void BoardLocationClass::move(const DirectionEnum &direction)
{
  switch(direction)
  {
    case UP:
      yCoord -= 1;
      break;
    case DOWN:
      yCoord += 1;
      break;
    case LEFT:
      xCoord -= 1;
      break;
    case RIGHT:
      xCoord += 1;
      break;
  }
  checkBoardBounds();
}
