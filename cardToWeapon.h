#ifndef _CARDTOWEAPON_H_
#define _CARDTOWEAPON_H_

#include "constants.h"
#include "enums.h"
#include "ExceptionClass.h"

inline WeaponEnum cardToWeapon(CardEnum card)
{
  if(card < NUMBER_OF_SUSPECTS ||
      card >= NUMBER_OF_SUSPECTS + NUMBER_OF_WEAPONS)
  {
    throw(ExceptionClass("The weapon for this card is an invalid value."));
  }
  return WeaponEnum(int(card));
}

#endif
