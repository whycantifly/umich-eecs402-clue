#ifndef _ROOMTOCARD_H_
#define _ROOMTOCARD_H_

#include "constants.h"
#include "enums.h"
#include "ExceptionClass.h"

inline CardEnum roomToCard(RoomEnum room)
{
  return CardEnum(int(room) + NUMBER_OF_SUSPECTS + NUMBER_OF_WEAPONS);
}

#endif
