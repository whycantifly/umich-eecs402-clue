#ifndef _BOARDLOCATIONCLASS_H_
#define _BOARDLOCATIONCLASS_H_

#include <QImage>
#include <QPoint>
#include <cmath>
#include <map>

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
    
    //Two BoardLocationClasse objects are equal if both the x and y 
    //coordinates are both equal.
    bool operator==(const BoardLocationClass &rhs) const
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

    //Two BoardLocationClass objects are not equal of either the x
    //or y coordinates are not equal.
    bool operator!=(const BoardLocationClass &rhs) const
    {
      return !(*this == rhs);
    }
    
    //The x-coordinate takes precendence when determining whether a
    //BoardLocationClass object is less or greater than another 
    //BoardLocationClass object; arbitrary rule implemented in order 
    //to avoid errors when dealing with set<BoardLocationClass> 
    //objects.
    bool operator<(const BoardLocationClass &rhs) const
    {
      return (xCoord < rhs.xCoord || (xCoord == rhs.xCoord && yCoord <
          rhs.yCoord));
    }
    
    bool operator>(const BoardLocationClass &rhs) const
    {
      return (xCoord > rhs.xCoord || (xCoord == rhs.xCoord && yCoord >
          rhs.yCoord));
    }
    
    bool operator<=(const BoardLocationClass &rhs) const
    {
      return !(*this > rhs);
    }

    bool operator>=(BoardLocationClass &rhs) const
    {
      return !(*this < rhs);
    }

    //Function Prototypes

    //Gets the tile color of the tile at this location on gameBoard.
    const QRgb getTileColor(
        const QImage &gameBoard   //Board to get the tile color from
        ) const;

    //Gets the tile type of the tile at this location on gameBoard.
    const TileTypeEnum getTileType(
        const QImage &gameBoard           //Board to get the tile type from
        ) const;

    //Gets the room of the tile at this location based on ROOM_TILE_RGB.
    const RoomEnum getRoom() const;

    //Gets the top left pixel of this tile.
    const QPoint getTopLeftPixel() const;

    //Gets the middle pixel of this tile.
    const QPoint getMiddlePixel() const;

    //Checks if the BoardLocationClass object belongs to a corner room.
    //Returns true if it belongs to a corner room, returns false if it belongs 
    //to a non-corner room, and throws an exception if the object does not
    //belong to a room.
    bool checkCornerRoom() const;

    //Checks if the object is within board bounds.  Returns true if it is
    //within board bounds, false otherwise.
    bool checkBoardBounds() const;

    //Moves this location over one tile. Throws an exception if this is an
    //invalid move.
    void move(
        const QImage &currentBoard,       //Board to move on,
        const DirectionEnum &direction    //Direction to move
        );

    //Overloaded function.  Returns the room for a door location.  Throws an
    //exception if the BoardLocationClass object is not a door or the door index
    //provided is an invalid door index.
    RoomEnum getRoomForDoor(BoardLocationClass doorLocation) const;

    RoomEnum getRoomForDoor(int doorIndex) const;

    //Gets the room a door belongs to.  Throws an exception if the 
    //BoardLocationClass object is not a door.
    RoomEnum getRoomDoor() const;

    //Gets an empty room tile for the room the BoardLocationClass object belongs
    //to.
    const BoardLocationClass getEmptyRoomTile(const QImage &currentBoard) const;

    //Gets the door index of the BoardLocationClass object.
    int getDoorIndex() const;

    //Gets the tile in the direction indicated direction relative to this
    //BoardLocationClass object.
    BoardLocationClass getTileInDir(const DirectionEnum direction) const;

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

    //Gets the distance from this location to the location indicated.
    int getDistanceTo(const BoardLocationClass location) const
    {
      return (abs(location.xCoord - xCoord) + abs(location.yCoord - yCoord));
    }
};

#endif
