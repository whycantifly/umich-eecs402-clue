//Name:       Elaine Pang, Taylor Tappe, Colin Zarzycki
//Date:       04/13/2012
//Assignment: EECS402 Project 5
//Purpose:    getSuspect global function definition

#ifndef _GETSUSPECT_H_
#define _GETSUSPECT_H_

#include "constants.h"
#include "enums.h"
#include "ExceptionClass.h"
#include "getCardType.h"

inline SuspectEnum getSuspect(CardEnum card)
{
  //Variable Declarations
  QString cardValue;

  if(getCardType(card) != SUSPECT_CARD)
  {
    cardValue.setNum(card);
    throw(ExceptionClass(cardValue + " is an invalid suspect card value."));
  }
  return SuspectEnum(int(card));
}

#endif
