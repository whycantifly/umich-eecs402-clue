//Name:       Elaine Pang, Taylor Tappe, Colin Zarzycki
//Date:       04/13/2012
//Assignment: EECS402 Project 5
//Purpose:    getCard global function definitions

#ifndef _GETCARD_H_
#define _GETCARD_H_

#include "constants.h"
#include "enums.h"
#include "ExceptionClass.h"

inline CardEnum getCard(SuspectEnum suspect)
{
  //Variable Declarations
  QString suspectValue;

  if(int(suspect) < 0 || int(suspect) >= NUMBER_OF_SUSPECTS)
  {
    suspectValue.setNum(suspect);
    throw(ExceptionClass(suspectValue + " is an invalid suspect value."));
  }
  return CardEnum(int(suspect));
}

inline CardEnum getCard(WeaponEnum weapon)
{
  //Variable Declarations
  QString weaponValue;

  if(int(weapon) < 0 || int(weapon) >= NUMBER_OF_WEAPONS)
  {
    weaponValue.setNum(weapon);
    throw(ExceptionClass(weaponValue + " is an invalid weapon value."));
  }
  return CardEnum(int(weapon) + NUMBER_OF_SUSPECTS);
}

inline CardEnum getCard(RoomEnum room)
{
  //Variable Declarations
  QString roomValue;

  if(int(room) < 0 || int(room) >= NUMBER_OF_ROOMS)
  {
    roomValue.setNum(room);
    throw(ExceptionClass(roomValue + " is an invalid room value."));
  }
  return CardEnum(int(room) + NUMBER_OF_SUSPECTS + NUMBER_OF_WEAPONS);
}

#endif
