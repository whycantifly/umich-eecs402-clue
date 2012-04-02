#ifndef _SUSPECTTOCARD_H_
#define _SUSPECTTOCARD_H_

#include "constants.h"
#include "enums.h"
#include "ExceptionClass.h"

inline CardEnum suspectToCard(SuspectEnum suspect)
{
  if(int(suspect) >= NUMBER_OF_SUSPECTS)
  {
    throw(ExceptionClass("That suspect does not have a card value."));
  }
  return CardEnum(int(suspect));
}

#endif
