#ifndef _GETWEAPON_H_
#define _GETWEAPON_H_

#include "constants.h"
#include "enums.h"
#include "ExceptionClass.h"
#include "getCardType.h"

inline WeaponEnum getWeapon(CardEnum card)
{
  //Variable Declarations
  QString cardValue;

  if(getCardType(card) != WEAPON_CARD)
  {
    cardValue.setNum(card);
    throw(ExceptionClass(cardValue + " is an invalid weapon card value."));
  }
  return WeaponEnum(int(card) - NUMBER_OF_SUSPECTS);
}

#endif
