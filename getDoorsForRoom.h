//Name:       Elaine Pang, Taylor Tappe, Colin Zarzycki
//Date:       04/13/2012
//Assignment: EECS402 Project 5
//Purpose:    getDoorsForRoom global function prototype

#ifndef _GETDOORSFORROOM_H_
#define _GETDOORSFORROOM_H_

#include <set>

#include "enums.h"
#include "getStartingDoorIndex.h"

class BoardLocationClass;

std::set<BoardLocationClass> getDoorsForRoom(RoomEnum room);

#endif
