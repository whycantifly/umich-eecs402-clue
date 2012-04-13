//Name:       Elaine Pang, Taylor Tappe, Colin Zarzycki
//Date:       04/13/2012
//Assignment: EECS402 Project 5
//Purpose:    getRoom global function definition

#ifndef _GETROOM_H_
#define _GETROOM_H_

#include "constants.h"
#include "enums.h"
#include "ExceptionClass.h"
#include "getCardType.h"

inline RoomEnum getRoom(CardEnum card)
{
  //Variable Declarations
  QString cardValue;

  if(getCardType(card) != ROOM_CARD)
  {
    cardValue.setNum(card);
    throw(ExceptionClass(cardValue + " an invalid room card value."));
  }
  return RoomEnum(int(card) - NUMBER_OF_SUSPECTS - NUMBER_OF_WEAPONS);
}

#endif
