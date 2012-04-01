#ifndef _BOARDLOCATIONCLASS_H_
#define _BOARDLOCATIONCLASS_H_

#include <QImage>
#include <QPoint>

#include "enums.h"

class BoardLocationClass
{
  private:
    int xCoord;       //x-coordinate of the location
    int yCoord;       //y-coordinate of the location

  public:

    //Constructors

    //Constructor(); initializes all member variables to 0;
    BoardLocationClass()
    {
      xCoord = 0;
      yCoord = 0;
    }

    //Constructor(int, int); initializes member variables based on:
    //x (x-coordinate)
    //y (y-coordinate)
    BoardLocationClass(const int &x, const int &y)
    {
      xCoord = x;
      yCoord = y;
    }

    //Overloaded functions
    bool operator==(const BoardLocationClass &rhs)
    {
      if(xCoord == rhs.xCoord && yCoord == rhs.yCoord)
      {
        return true;
      }
      else
      {
        return false;
      }
    }

    //Function Prototypes

    //Gets the tile color of the tile at this location on gameBoard.
    const QRgb getTileColor(
        const QImage &gameBoard   //Board to get the tile color from
        ) const;

    //Gets the tile type of the tile at this location on gameBoard.
    const TileTypeEnum getTileType(
        const QImage &gameBoard   //Board to get the tile type from
        ) const;

    //Gets the room of the tile at this location based on ROOM_TILE_RGB.
    const RoomEnum getRoom() const;

    //Gets the top left pixel of this tile.
    const QPoint getTopLeftPixel() const;

    //Gets the middle pixel of this tile.
    const QPoint getMiddlePixel() const;

    //Thows an exception if the location is outside of the bounds of the board.
    void checkBoardBounds();

    //Moves this location over one tile; throws an exception if this exceeds
    //the bounds of the board.
    void move(
        const DirectionEnum &direction    //Direction to move
        );

    //If the tile is a door tile, gets the room that it belongs to.  If the tile
    //is not a room tile, throws an exception.
    RoomEnum getRoomDoor() const;

    //Inline functions

    //Gets the x-coordinate.
    int getXCoord() const
    {
      return xCoord;
    }

    //Gets the y-coordinate.
    int getYCoord() const
    {
      return yCoord;
    }
};

#endif
