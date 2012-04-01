#ifndef _CARDTOSUSPECT_H_
#define _CARDTOSUSPECT_H_

#include "constants.h"
#include "enums.h"
#include "ExceptionClass.h"

inline SuspectEnum cardToSuspect(CardEnum card)
{
  if(card < 0 || card >= NUMBER_OF_SUSPECTS)
  {
    throw(ExceptionClass("The suspect for this card is an invalid value."));
  }
  return SuspectEnum(int(card));
}

#endif
