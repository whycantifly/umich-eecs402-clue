#ifndef _GETCARDTYPE_H_
#define _GETCARDTYPE_H_

#include "constants.h"
#include "enums.h"

inline CardTypeEnum getCardType(CardEnum card)
{
  //Variable Declarations
  QString cardValue;

  if(int(card) < NUMBER_OF_SUSPECTS)
  {
    return SUSPECT_CARD;
  }
  else if(int(card) < NUMBER_OF_SUSPECTS + NUMBER_OF_WEAPONS)
  {
    return WEAPON_CARD;
  }
  else if(int(card) < NUMBER_OF_CARDS)
  {
    return ROOM_CARD;
  }
  else
  {
    cardValue.setNum(card);
    throw(ExceptionClass(cardValue + " is an invalid card value."));
  }
}

#endif
