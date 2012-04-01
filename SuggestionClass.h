#ifndef _SUGGESTIONCLASS_H_
#define _SUGGESTIONCLASS_H_

#include "enums.h"
#include "DeckClass.h"

class SuggestionClass
{
  protected:
    CardEnum suspectCard;
    CardEnum weaponCard;
    CardEnum roomCard;

  public:
    //Constructors
    SuggestionClass(int suspect = 0, int weapon = 0, int room = 0)
    {
      suspectCard = CardEnum(suspect);
      weaponCard = CardEnum(weapon);
      roomCard = CardEnum(room);
    }

    SuggestionClass(CardEnum suspect, CardEnum weapon, CardEnum room)
    {
      suspectCard = suspect;
      weaponCard = weapon;
      roomCard = room;
    }

    //Overloaded operators
    bool operator==(const SuggestionClass &rhs) const
    {
      return(suspectCard == rhs.suspectCard && weaponCard == rhs.weaponCard &&
          roomCard == rhs.roomCard);
    }

    bool operator!=(const SuggestionClass &rhs) const
    {
      return !(*this == rhs);
    }

    //Inline functions
    const CardEnum getSuspectCard() const
    {
      return suspectCard;
    }
    const CardEnum getWeaponCard() const
    {
      return weaponCard;
    }
    const CardEnum getRoomCard() const
    {
      return roomCard;
    }
};

#endif
