#ifndef _WEAPONTOCARD_H_
#define _WEAPONTOCARD_H_

#include "constants.h"
#include "enums.h"
#include "ExceptionClass.h"

inline CardEnum weaponToCard(WeaponEnum weapon)
{
  return CardEnum(int(weapon) + NUMBER_OF_SUSPECTS);
}

#endif
