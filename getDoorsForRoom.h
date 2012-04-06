#ifndef _GETDOORSFORROOM_H_
#define _GETDOORSFORROOM_H_

#include <set>

#include "enums.h"
#include "getStartingDoorIndex.h"

class BoardLocationClass;

std::set<BoardLocationClass> getDoorsForRoom(RoomEnum room);

#endif
