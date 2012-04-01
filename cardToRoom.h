#ifndef _CARDTOROOM_H_
#define _CARDTOROOM_H_

#include "constants.h"
#include "enums.h"
#include "ExceptionClass.h"

inline RoomEnum cardToRoom(CardEnum card)
{
  if(card < NUMBER_OF_SUSPECTS + NUMBER_OF_WEAPONS ||
      card >= NUMBER_OF_SUSPECTS + NUMBER_OF_WEAPONS + NUMBER_OF_ROOMS)
  {
    throw(ExceptionClass("The room for this card is an invalid value."));
  }
  return RoomEnum(int(card) - NUMBER_OF_SUSPECTS - NUMBER_OF_WEAPONS);
}

#endif
