#ifndef _SUSPECTTOCARD_H_
#define _SUSPECTTOCARD_H_

#include "constants.h"
#include "enums.h"
#include "ExceptionClass.h"

inline CardEnum suspectToCard(SuspectEnum suspect)
{
  return CardEnum(int(suspect));
}

#endif
