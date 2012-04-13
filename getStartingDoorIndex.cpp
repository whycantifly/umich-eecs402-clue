//Name:       Elaine Pang, Taylor Tappe, Colin Zarzycki
//Date:       04/13/2012
//Assignment: EECS402 Project 5
//Purpose:    getStartingDoorIndex global function definition

#include "getStartingDoorIndex.h"
#include "constants.h"
#include "enums.h"

int getStartingDoorIndex(RoomEnum room)
{
  int runningDoorCount = 0;

  for(int i = 0; i < int(room); i++)
  {
    runningDoorCount += NUMBER_OF_DOORS[i];
  }

  return runningDoorCount;
}
