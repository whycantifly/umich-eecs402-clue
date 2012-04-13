//Name:       Elaine Pang, Taylor Tappe, Colin Zarzycki
//Date:       04/13/2012
//Assignment: EECS402 Project 5
//Purpose:    getDoorsForRoom global function definition

#include <set>

#include "getDoorsForRoom.h"
#include "constants.h"
#include "enums.h"
#include "getStartingDoorIndex.h"

class BoardLocationClass;

using namespace std;

set<BoardLocationClass> getDoorsForRoom(RoomEnum room)
{
  set<BoardLocationClass> doors;

  for(int i = 0; i < NUMBER_OF_DOORS[room]; i++)
  {
    doors.insert(DOOR_LOCATIONS[getStartingDoorIndex(room) + i]);
  }

  return doors;
}
